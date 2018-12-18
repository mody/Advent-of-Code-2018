#!/usr/bin/perl
use warnings;
use strict;

# use Data::Dumper;

# dabAcCaCBAcCcaDA

my %lower = map { $_ => 1 } ('a'..'z');
my $end = 0;

my $line = <>;
chomp($line);

my @input = split(//, $line);

while($end == 0) {
    $end = 1;
    foreach my $i (0..(scalar @input - 2)) {
        my $c = $input[$i];
        my $o;
        if (exists $lower{$c}) {
            $o = uc($c);
        } else {
            $o = lc($c);
        }
        if ($input[$i+1] eq $o) {
            splice(@input, $i, 2);
            $end = 0;
            last;
        }
    }
}

print scalar @input, "\n";
