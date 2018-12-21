#!/usr/bin/perl
use warnings;
use strict;

#use Data::Dumper;

sub sum_distances {
    my ($x, $y, $points) = @_;
    my $sum = 0;
    foreach my $p (@$points ) {
        $sum += abs($p->[0] - $x) + abs($p->[1] - $y);
    }
    if ($sum >= 10000) {
        return 0;
    } else {
        return 1
    };
}

my @points;

my $minX;
my $minY;
my $maxX;
my $maxY;

while(my $line = <>) {
    chomp($line);
    die "invalid input!" if not $line =~ /(\d+),[^0-9]+(\d+)$/;
    $minX = ($minX and $minX < $1) ? $minX : $1;
    $minY = ($minY and $minY < $2) ? $minY : $2;
    push(@points, [$1, $2]);
}

foreach my $p (@points) {
    @$p[0] -= $minX;
    @$p[1] -= $minY;
    $maxX = ($maxX and $maxX > @$p[0]) ? $maxX : @$p[0];
    $maxY = ($maxY and $maxY > @$p[1]) ? $maxY : @$p[1];
}


my $sum = 0;

for (my $y = 0; $y < $maxY; $y++) {
    for (my $x = 0; $x < $maxX; $x++) {
        $sum += sum_distances($x, $y, \@points);
    }
}

print "area: $sum\n";
