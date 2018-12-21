#!/usr/bin/perl
use warnings;
use strict;

#use Data::Dumper;

sub closest {
    my ($x, $y, $points) = @_;
    my @d;
    while ( my ($i, $p) = each @$points ) {
        push @d, [$i, abs($p->[0] - $x) + abs($p->[1] - $y)];
    }
    @d = sort { $a->[1] <=> $b->[1] } @d;
    if ($d[0][1] == $d[1][1]) {
        return -1;
    }
    return $d[0][0];
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

my @a;
my @ex;

for (my $y = 0; $y < $maxY; $y++) {
    for (my $x = 0; $x < $maxX; $x++) {
        my $c = closest($x, $y, \@points);
        if ($c >= 0) {
            $a[$c]++;
        }
        if ($x == 0 or $x == ($maxX-1) or $y == 0 or $y == ($maxY-1)) {
            $ex[$c] = 1;
        }
    }
}

foreach my $i (@ex) {
    $a[$i] = undef;
}

my %h;
my @c = ('A'..'Z');
while (my ($i, $c) = each @c) {
    $h{$c} = $a[$i];
}

my $max = 0;
foreach my $v (values %h) {
    next if not $v;
    $max = $max < $v ? $v : $max;
}

print $max, "\n";
