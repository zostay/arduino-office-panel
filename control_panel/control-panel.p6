#!/usr/bin/env perl6
use v6;

use RPi::Wiring::Pi;

constant QUIETUDE_DURATION = 30;
constant BLINK_FREQ        = 0.5;

class Pin {
    has $.pin is required;
    method mode($mode) { pinMode($.pin, $mode) }
    method read() { digitalRead($.pin) }
    method write($state) { digitalWrite($.pin, $state) }

    method toggle() {
        if digitalRead($.pin) == HIGH {
            digitalWrite($.pin, LOW);
        }
        else {
            digitalWrite($.pin, HIGH);
        }
    }

    method events() {
        Supply.interval(0.01).map({
            state $prev;
            my $state = self.read;

            LEAVE { $prev = $state };

            if !$prev.defined || $prev != $state {
                $state;
            }
            else {
                Mu
            }
        }).grep({ .defined });
    }
}

enum SignalState ( OffAir => 0, OnAir => 1, Quietude => 2 );

class LightPanel {
    has Str $.panel-ip is required;
    has Bool $.dry-run = False;
    has SignalState $.signal-state = OffAir;

    has IO::Socket::Async $!sock .= udp;

    method is-on-air { $.signal-state != OffAir }
    method is-quiet  { $.signal-state == Quietude }

    method go-on-air {
        return if $.is-on-air;
        $!signal-state = OnAir;
        self.send-state-update();
    }

    method end-quietude {
        return unless $.is-quiet;
        if $.is-on-air {
            $!signal-state = OnAir;
        }
        else {
            $!signal-state = OffAir;
        }
        self.send-state-update();
    }

    method go-off-air {
        $!signal-state = OffAir;
        self.send-state-update();
    }

    method go-quiet {
        return unless $.is-on-air;
        $!signal-state = Quietude;
        self.send-state-update();
    }

    method send-state-update() {
        return self.dry-run-state-update() if $.dry-run;
        $!sock.write-to($!panel-ip, 10101, Blob.new([ $!signal-state.Int ]));
    }

    method dry-run-state-update() {
        say "Sending state: $.signal-state";
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

class Blinker {
    has Pin $.light is required;
    has Tap $.blinky-blinky;

    method start {
        $!blinky-blinky = Supply.interval(0.5).tap({ $.light.toggle });
    }

    method stop {
        with $!blinky-blinky {
            $!blinky-blinky.close;
            $.light.write(LOW);
        }
    }
}

sub MAIN(:$dry-run = False, :$panel-ip!) {
    wiringPiSetup();

    my LightPanel   $light         .= new(:$dry-run, :$panel-ip);
    my ControlPanel $control       .= new;
    my Blinker      $quiet-blinker .= new(light => $control.quiet-light);
    my $quietude-event;

    react {
        whenever $control.on-air-events -> $on-air {
            when $on-air {
                $light.go-on-air;
            }
            default {
                $quiet-blinker.stop;
                $light.go-off-air;
            }
        }

        whenever $control.quiet-events -> $quiet {
            when $quiet {
                if $control.is-on-air {
                    $quiet-blinker.start;
                    my $expected-event = $quietude-event = rand;
                    Promise.at(now + QUIETUDE_DURATION).then({
                        if ($quietude-event == $expected-event) {
                            $light.end-quietude;
                            $quiet-blinker.stop;
                        }
                    });
                    $light.go-quiet;
                }
            }
        }
    }
}
