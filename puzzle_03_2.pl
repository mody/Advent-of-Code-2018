#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

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

my @m = ();

while(<>) {
    chomp;
    if (not m/^#(\d+) @ (\d+),(\d+): (\d+)x(\d+)$/) {
        die "invalid input!";
    }
    my ($id, $x, $y, $w, $h) = ($1, $2, $3, $4, $5);

    foreach my $j ($y .. ($y-1+$h)) {
        if (not exists $m[$j]) {
            $m[$j] = ();
        }
        my $r = \@{$m[$j]};

        foreach my $i ($x .. ($x-1+$w)) {
            if (not exists $r->[$i]) {
                $r->[$i] = ();
            }

            my $a = \@{$r->[$i]};
            push(@{$a}, $id);
        }
    }
}

my %max = ();

foreach my $r (@m) {
    foreach my $x (@{$r}) {
        next if not $x;
        my $count = scalar @{$x};
        foreach my $id (@{$x}) {
            $max{$id} = (not exists $max{$id} or $max{$id} < $count) ? $count : $max{$id};
        }
    }
}

print Dumper \%max;
