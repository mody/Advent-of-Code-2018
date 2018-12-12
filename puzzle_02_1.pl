#!/usr/bin/perl
use warnings;
use strict;

my $twos = 0;
my $threes = 0;

while(<>) {
    chomp;
    my @line = sort(split(//, $_));
    my %counts = ();
    foreach my $c (@line) {
        if (exists $counts{$c}) {
            $counts{$c} += 1;
        } else {
            $counts{$c} = 1;
        }
    }
    foreach my $c (values %counts) {
        if ($c == 2) {
            $twos += 1;
            last;
        }
    }
    foreach my $c (values %counts) {
        if ($c == 3) {
            $threes += 1;
            last;
        }
    }
}
print "twos: $twos, threes: $threes, result: ", $twos*$threes, "\n";
