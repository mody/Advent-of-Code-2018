#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

my %h;

# Step C must be finished before step A can begin.
while(<>) {
    chomp;
    my @a = split(/ /);
    my ($x, $y) = ($a[1], $a[7]);
    push @{$h{$x}}, $y;
}

my @result;

while (scalar keys %h != 1) {
    my @deps = map {@$_} values %h;

    my $v;

    foreach my $k (sort keys %h) {
        my $l = scalar grep { $k eq $_} @deps;
        if ($l == 0) {
            $v = $k;
            push @result, $v;
            last;
        }
    }

    delete $h{$v};

    while (my ($k, $v) = each %h) {
        $v = grep {$k ne $_} @$v;
    }
}

while (my ($k, $v) = each %h) {
    push @result, $k;
    map { push @result, $_ } @$v;
}

print @result, "\n";
