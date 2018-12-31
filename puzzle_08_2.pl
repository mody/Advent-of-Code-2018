#!/usr/bin/perl
use warnings;
use strict;

no warnings 'recursion';

sub read_node {
    my ($i, $level) = @_;
    my $children = shift(@$i);
    my $meta_count = shift(@$i);

    my @values;
    my @meta;

    for (my $child = 0; $child < $children; ++$child) {
        push @values, read_node($i, $level+1);
    }

    for (my $m = 0; $m < $meta_count; ++$m) {
        push @meta, shift(@$i);
    }

    my $sum = 0;

    if ($children == 0) {
        map { $sum += $_ } @meta;
    } else {
        foreach my $ii (@meta) {
            $ii -= 1;
            next if $ii < 0 or $ii >= scalar @values; # index out of bounds
            $sum += $values[$ii];
        }
    }

    return $sum;
}

chomp(my $line = <>);
my @input = split(/ /, $line);

my $sum = read_node(\@input, 0);

print("SUM: $sum\n");
