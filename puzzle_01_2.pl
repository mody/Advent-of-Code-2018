#!/usr/bin/perl
use warnings;

my $i = 0;
my %x = ();

while(<>) {
    chomp;
    $i += $_;
    last if exists $x{$i};
    $x[$i] = 1;
}
print "result: ", $i,"\n";
