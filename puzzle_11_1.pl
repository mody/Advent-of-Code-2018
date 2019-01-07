#!/usr/bin/perl
use warnings;
use strict;

use POSIX;
use Data::Dumper;

my $DIM = 300;
my @pole = ();

my $SERIAL = 9445;
foreach my $y (0..($DIM-1)) {
    foreach my $x (0..($DIM-1)) {
        $pole[$y][$x] = floor((((($x + 10) * $y) + $SERIAL) * ($x + 10) % 1000) / 100) - 5;
    }
}

my ($top_x, $top_y, $top_val) = (0, 0, 0);

foreach my $y (0..($DIM-3)) {
    foreach my $x (0..($DIM-3)) {
        my $val = $pole[$y][$x] +
                  $pole[$y][$x+1] +
                  $pole[$y][$x+2] +
                  $pole[$y+1][$x] +
                  $pole[$y+1][$x+1] +
                  $pole[$y+1][$x+2] +
                  $pole[$y+2][$x] +
                  $pole[$y+2][$x+1] +
                  $pole[$y+2][$x+2];
        if ($top_val < $val) {
            $top_val = $val;
            $top_x = $x;
            $top_y = $y;
        }
    }
}

print "$top_x,$top_y = $top_val\n";
