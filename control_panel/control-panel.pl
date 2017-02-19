#!/usr/bin/perl
use v5.24;
use warnings;

use RPi::WiringPi;
use Time::HiRes qw( sleep );

use constant DEBOUNCE_GO   => 10;
use constant DEBOUNCE_FAIL => 10;

use constant OFF_AIR  => 0;
use constant ON_AIR   => 1;
use constant QUIETUDE => 2;

my $pi = RPi::WiringPi->new;

my $on_air_button = $pi->pin(21);
my $quiet_button  = $pi->pin(20);
my $quiet_light   = $pi->pin(16);

$on_air_button->mode(INPUT);
$on_air_button->interrupt_set(EDGE_RISING, 'begin_on_air');
$on_air_button->interrupt_set(EDGE_FALLING, 'end_on_air');
$quiet_button->mode(INPUT);
$quiet_button->interrupt_set(EDGE_RISING, 'begin_quietude');
$quiet_light->mode(OUTPUT);


my $on_air_state = ($on_air_button->read == HIGH);
my $quiet_state  = ($quiet_button->read  == HIGH);
my $end_quietude = 0;

my $signal_state;

my %debouncer;
sub debounce_switch {
    my ($which, $pin, $expect, $callback, $continuous) = @_;
    $debouncer{ $which } = {
        go     => 0,
        fail   => 0,
        pin    => $pin,
        expect => $expect,
        cb     => $callback,
        cont   => !!$continuous,
    };
}

sub debouncer_check {
    for my $key (keys %debouncer) {
        my $t     = $debouncer{$key};
        my $state = $t->{pin}->read;

        if ($state == $t->{expect}) {
            $t->{go}++;
            $t->{fail} = 0;
        }
        else {
            $t->{go} = 0;
            $t->{fail}++;
        }

        # Success!
        if ($t->{go} >= DEBOUNCE_GO) {
            $t->{cb}->();

            # Continuously call the callback until we fail
            delete $debouncer{$key} unless $t->{cont};
        }

        # Fail!
        elsif ($t->{fail} >= DEBOUNCE_FAIL) {
            delete $debouncer{$key};
        }
    }
}

sub begin_on_air {
    debounce_switch('begin_on_air', $on_air_button, HIGH, sub {
        $on_air_state = 1;
        send_state_update();
    });
}

sub end_on_air {
    debounce_switch('end_on_air', $on_air_button, LOW, sub {
        $on_air_state = 0;
        send_state_update();
    });
}

sub begin_quietude {
    debounce_switch('begin_quietude', $quiet_button, HIGH, sub {
        $quietude = 1;
        $end_quietude = time + 30;
    }, 'continuous');
}

sub send_state_update {
    die "IMPLEMENT ME!";
}

sub update_signal_state {
    my ($force) = @_;

    my $old_state = $signal_state;

    if ($on_air_state) {
        if ($quiet_state) {
            $signal_state = QUIETUDE;
        }
        else {
            $signal_state = ON_AIR;
        }
    }
    else {
        $signal_state = OFF_AIR;
    }

    if ($force_update || $old_state != $signal_state) {
        send_state_update();
    }
}

update_signal_state('FORCE');
while (1) {
    debouncer_check();

    if ($end_quietude && time > $end_quietude) {
        $quietude_state = $end_quietude = 0;
    }

    sleep 0.001;
}
