#!/usr/bin/env perl6
use v6;

constant UPDATE_HZ = 10;
constant GRID_W = 8;
constant GRID_H = 8;

my @pile-colors = (0, 0, 0), (0, 0, 255), (255, 255, 0), (255, 0, 0);
my @piles-grid = 0 xx GRID_W * GRID_H;

sub piles-grid($x, $y) { return-rw @piles-grid[$x + $y * 8] }
sub term:<randx> { floor(rand * GRID_W) }
sub term:<randy> { floor(rand * GRID_H) }

sub piles-grid-neighbors($x, $y) {
    gather {
        take ($x-1, $y) if $x > 0;
        take ($x, $y-1) if $y > 0;
        take ($x+1, $y) if $x < GRID_W-1;
        take ($x, $y+1) if $y < GRID_H-1;
    }
}

sub fall-over($x, $y) {
    my $here := piles-grid($x, $y);
    if ($here > 3) {
        $here -= 4;
        for piles-grid-neighbors($x, $y) -> ($x, $y) {
            piles-grid($x, $y)++;
            fall-over($x, $y);
        }
    }
}

my $udp = IO::Socket::Async.udp;

react {
    whenever Supply.interval(1/UPDATE_HZ) {
        my $x = randx;
        my $y = randy;
        piles-grid($x, $y)++;
        fall-over($x, $y);

        my @output = 0x13;
        for ^GRID_H X ^GRID_W -> ($y, $x) {
            my $here := piles-grid($x, $y);
            push @output, |@pile-colors[ $here ];
        }

        dd @output;

        $udp.write-to('127.0.0.1', 10101, buf8.new(@output));
    }
}
