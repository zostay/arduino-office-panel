#!/usr/bin/env perl6
use v6;

use RPi::Wiring::Pi;

constant QUIETUDE_DUR  = 30;
constant BLINK_FREQ    = 0.5;

constant OFF_AIR       = 0;
constant ON_AIR        = 1;
constant QUIETUDE      = 2;

class Pin {
    has $.pin is required;
    method mode($mode) { pinMode($.pin, $mode) }
    method read() { digitalRead($.pin) }
    method write($state) { digitalWrite($.pin, $state) }

    method events() {
        supply {
            wiringPiISR($.pin, INT_EDGE_BOTH, sub {
                emit self.read;
            });
        }
    }
}

enum SignalState ( OffAir => 0, OnAir => 1, Quietude => 2 );

class LightPanel {
    has Bool $.dry-run = False;
    has SignalState $.signal-state = OffAir;

    method is-on-air { $.signal-state != OffAir }
    method is-quiet  { $.signal-state == Quietude }

    method go-on-air {
        return if not $end-quietude and $.is-on-air;
        $.signal-state = OnAir;
        send-state-update();
    }

    method end-quietude {
        return unless $.is-quiet;
        if $.is-on-air {
            $.signal-state = OnAir;
        }
        else {
            $.signal-state = OffAir;
        }
        send-state-update();
    }

    method go-off-air {
        $.signal-state = OffAir;
        send-state-update();
    }

    method go-quiet {
        return unless $.is-on-air;
        $.signal-state = Quietude;
        send-state-update();
    }

    method send-state-update() {
        return dry-run-state-update() if $dry-run;
        ...
    }

    method dry-run-state-update() {
        say "Sending state: $signal_state";
    }
}

class ControlPanel {
    constant ON_AIR_BUTTON = 3;
    constant QUIET_BUTTON  = 2;
    constant QUIET_LIGHT   = 0;

    has $.on-air-button = Pin.new(:pin(ON_AIR_BUTTON));
    has $.quiet-button  = Pin.new(:pin(QUIET_BUTTON));
    has $.quiet-light   = Pin.new(:pin(QUIET_LIGHT));

    submethod TWEAK {
        $!on-air-button.mode(INPUT);
        $!quiet-button.mode(INPUT);
        $!quiet-light.mode(OUTPUT);

        $!quiet-light.write(LOW);
    }

    method is-on-air() { $.on-air-button.read == HIGH }
    method is-quiet()  { $.quiet-button.read  == HIGH }

    method on-air-events() { $.on-air-button.events.map(* == HIGH) }
    method quiet-events() { $.quiet-button.events.map(* == HIGH) }
}

sub update_signal_state(:$force, :$dry-run) {
    my $old_state = $signal_state;

    if $on_air_state && $on_air_count >= 2 {
        if $quiet_state && $quiet_count >= 2 {
            $signal_state = QUIETUDE;
        }
        else {
            $signal_state = ON_AIR;
        }
    }
    elsif $on_air_count >= 2 {
        $signal_state = OFF_AIR;
    }

    if $force || ($old_state != QUIETUDE && $old_state != $signal_state) {
        if $signal_state == QUIETUDE {
            $end_quietude = now + QUIETUDE_DUR;
            $next_blink   = now + BLINK_FREQ;
            $blink_to     = HIGH;
        }

        elsif $old_state == QUIETUDE && $signal_state == ON_AIR {
            $signal_state = QUIETUDE;
        }

        elsif $old_state == QUIETUDE && $signal_state == OFF_AIR {
            $end_quietude = $next_blink = 0;
            $quiet_light.write(LOW);
        }

        send_state_update(:$dry-run);

    }
}

sub read_state(:$button, :$count! is rw, :$state! is rw) {
    my $now_state = $button.read == HIGH;
    if ($state == $now_state) {
        $count++;
    }
    else {
        $state = $now_state;
        $count = 0;
    }
}

sub MAIN(:$dry-run) {
    wiringPiSetup();

    my LightPanel   $light         .= new(:$dry-run);
    my ControlPanel $control       .= new;
    my Blinker      $quiet-blinker .= new($light.quiet-light);

    react {
        whenever $control.on-air-events {
            when True  {
                $light.go-on-air;
            }
            when False {
                $quiet-blinker.stop;
                $light.go-off-air;
            }
        }

        whenever $control.quiet-events {
            when True  {
                if $control.is-on-air {
                    $quiet-blinker.start;
                    my $at = Supply.interval(QUIETUDE_DURATION);
                    whenever $at {
                        $at.done;
                        $light.end-quietude;
                        $quiet-blinker.stop;
                    }
                    $light.go-quiet;
                }
            }
        }

        whenever $quiet-blinker.toggle-events {
            $quiet-blinker.toggle;
        }


    }

    update_signal_state(:force, :$dry-run);
    loop {
        read_state(
            button => $on_air_button,
            count  => $on_air_count,
            state  => $on_air_state,
        );

        read_state(
            button => $quiet_button,
            count  => $quiet_count,
            state  => $quiet_state,
        );

        if $end_quietude && now > $end_quietude {
            $quiet_state = $end_quietude = 0;
            $quiet_count = 2;
        }

        update_signal_state(:$dry-run);

        if $signal_state == QUIETUDE {
            if now >= $next_blink {
                $quiet_light.write($blink_to);
                $next_blink = now + BLINK_FREQ;
                $blink_to   = $blink_to == LOW ?? HIGH !! LOW;
            }

            if now >= $end_quietude {
                $quiet_light.write(LOW);
                $next_blink = $end_quietude = 0;
                $signal_state = ON_AIR;
            }
        }

        sleep 0.1;
    }
}
