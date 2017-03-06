#!/usr/bin/env perl
use v5.24;
use warnings;

use RPi::WiringPi;
use Time::HiRes qw( sleep );

use constant QUIETUDE_DUR => 30;
use constant BLINK_FREQ   => 0.5;

use constant OFF_AIR  => 0;
use constant ON_AIR   => 1;
use constant QUIETUDE => 2;

my $pi = RPi::WiringPi->new;

my $on_air_button = $pi->pin(3);
my $quiet_button  = $pi->pin(2);
my $quiet_light   = $pi->pin(0);

$on_air_button->mode(INPUT);
$quiet_button->mode(INPUT);
$quiet_light->mode(OUTPUT);

my $on_air_state = ($on_air_button->read == HIGH);
my $on_air_count = 0;
my $quiet_state  = ($quiet_button->read  == HIGH);
my $quiet_count  = 0;
my $end_quietude = 0;
my $next_blink   = 0;
my $blink_to     = LOW;

my $signal_state = OFF_AIR;

sub send_state_update {
    die "IMPLEMENT ME!";
}

sub update_signal_state {
    my ($force) = @_;

    my $old_state = $signal_state;

    if ($on_air_state && $on_air_count >= 2) {
        if ($quiet_state && $quiet_count >= 2) {
            $signal_state = QUIETUDE;
        }
        else {
            $signal_state = ON_AIR;
        }
    }
    elsif ($on_air_count >= 2) {
        $signal_state = OFF_AIR;
    }

    if ($force_update || $old_state != $signal_state) {
        send_state_update();

        if ($signal_state == QUIETUDE) {
            $end_quietude = time + QUIETUDE_DUR;
            $next_blink   = time + BLINK_FREQ;
            $blink_to     = HIGH;
        }

        elsif ($old_state == QUIETUDE) {
            $end_quietude = 0;
            $next_blink   = 0;
            $blink_to     = LOW;
        }
    }
}

sub read_state {
    my (%opt) = @_;

    my $button = $opt{button};
    my $count  = $opt{count};
    my $state  = $opt{state};

    my $now_state = $button->read == HIGH;
    if ($$state == $now_state) {
        $$count++;
    }
    else {
        $$state = $now_state;
        $$count = 0;
    }
}

update_signal_state('FORCE');
while (1) {
    read_state({
        button => $on_air_button,
        count  => \$on_air_count,
        state  => \$on_air_state,
    });

    read_state({
        button => $quiet_button,
        count  => \$quiet_count,
        state  => \$quiet_state,
    });

    if ($end_quietude && time > $end_quietude) {
        $quietude_state = $end_quietude = 0;
    }

    update_signal_state();

    if ($signal_state == QUIETUDE && time <= $next_blink) {
        $quiet_light->write($blink_to);
        $next_blink = time + BLINK_FREQ;
        $blink_to   = $blink_to == LOW ? HIGH : LOW;
    }

    sleep 0.1;
}
