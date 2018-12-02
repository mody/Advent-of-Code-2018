#!/usr/bin/perl
use warnings;

my $i = 0;

while(<>) {
    chomp;
    $i += $_
}
print "result: ", $i,"\n";
