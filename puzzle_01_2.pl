#!/usr/bin/perl
use warnings;
use strict;

my @a = ();

while(<>) {
    chomp;
    push(@a, $_);
}

my $i = 0;
my %x = ();

while(1) {
    foreach my $j (@a) {
        $i += $j;
        print "$i\t$j\n";
        if(defined $x{$i}) {
            print("Result: $i\n");
            return;
        }
        $x{$i} = $i;
    }
}
