#!/usr/bin/perl
use warnings;
use strict;

#1 @ 669,271: 17x11
#2 @ 153,186: 20x26
#3 @ 186,838: 28x11
#4 @ 119,248: 18x13
#5 @ 57,843: 14x11
#6 @ 868,833: 18x20
#7 @ 225,38: 26x20
#8 @ 208,673: 29x22
#9 @ 25,877: 23x13
#10 @ 406,555: 11x25

my @map = ();

while(<>) {
    chomp;
    if (not m/^#(\d+) @ (\d+),(\d+): (\d+)x(\d+)$/) {
        die "invalid input!";
    }
    my ($x, $y, $w, $h) = ($2, $3, $4, $5);

    foreach my $j ($y .. ($y-1+$h)) {
        if (not exists $map[$j]) {
            $map[$j] = ();
        }
        foreach my $i ($x .. ($x-1+$w)) {
            $map[$j][$i] += 1;
        }
    }
}

my $count = 0;
foreach my $r (@map) {
    foreach my $x (@$r) {
        next if (not $x);
        if ($x > 1) {
            $count++;
        }
    }
}

print $count, "\n";
