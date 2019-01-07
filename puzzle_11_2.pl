#!/usr/bin/perl
use warnings;
use strict;

use POSIX;
use Data::Dumper;

sub get_x_y_q {
    my ($x, $y, $q) = @_;
    my ($dx, $dy) = (-1, 0);
    return sub {
        if (++$dx == $q) {
            $dx = 0;
            ++$dy;
        }
        if ($dy == $q) {
            return undef;
        }
        my $rx = $x + $dx;
        my $ry = $y + $dy;
        return ($rx, $ry);
    }
}


my $DIM = 300;
my @pole = ();

my $SERIAL = 9445;
foreach my $y (0..($DIM-1)) {
    foreach my $x (0..($DIM-1)) {
        $pole[$y][$x] = floor((((($x + 10) * $y) + $SERIAL) * ($x + 10) % 1000) / 100) - 5;
    }
}

my ($top_x, $top_y, $top_val, $top_q) = (0, 0, 0, 0);

foreach my $q (1..$DIM-1) {
    foreach my $y (0..($DIM-$q)) {
        foreach my $x (0..($DIM-$q)) {
            my $val = 0;
            my $it = get_x_y_q($x, $y, $q);
            while(my ($xx, $yy) = $it->()) {
                last if not defined $xx or not defined $yy;
                $val += $pole[$yy][$xx];
            }
            if ($top_val < $val) {
                $top_val = $val;
                $top_x = $x;
                $top_y = $y;
                $top_q = $q;
            }
        }
    }
    print "$q: $top_x,$top_y,$top_q = $top_val\n";
}

print "FINAL: $top_x,$top_y,$top_q = $top_val\n";
