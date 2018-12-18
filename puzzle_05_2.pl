#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

# dabAcCaCBAcCcaDA

my %lower = map { $_ => 1 } ('a'..'z');
my $end = 0;

my $line = <>;
chomp($line);

my @base_input = split(//, $line);

my $min = scalar @base_input;

foreach my $x ('a'..'z') {
    my @input = grep { ($x ne lc($_)) } @base_input;

    my $l = scalar @input - 1;
    for (my $i=0; $i < $l; $i++) {
        my $c = $input[$i];
        my $o;
        if (exists $lower{$c}) {
            $o = uc($c);
        } else {
            $o = lc($c);
        }
        if ($input[$i+1] eq $o) {
            splice(@input, $i, 2);
            $l = scalar @input - 1;
            $i -= 2;
            if ($i == $l) {
                last;
            }
        }
    }
    $min = $min > scalar @input ? scalar @input : $min
}

print "min: $min\n";
