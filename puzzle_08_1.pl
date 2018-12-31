#!/usr/bin/perl
use warnings;
use strict;

no warnings 'recursion';

sub read_node {
    my ($i, $sum) = @_;
    my $children = shift(@$i);
    my $meta_count = shift(@$i);

    for (my $child = 0; $child < $children; ++$child) {
        read_node($i, $sum);
    }

    for (my $m = 0; $m < $meta_count; ++$m) {
        $$sum += shift(@$i);
    }
}

chomp(my $line = <>);
my @input = split(/ /, $line);

my $sum = 0;
read_node(\@input, \$sum);

print("SUM: $sum\n");
