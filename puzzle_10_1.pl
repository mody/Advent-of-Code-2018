#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

my @points;
my $min_area;
my $tick = 0;

while(<>) {
    chomp;
    die "invalid input [$_]" if not m/^position=< *([\d-]+), *([\d-]+)> velocity=< *([\d-]+), *([\d-]+)>/;
    my @p = [$1, $2, $3, $4];
    push @points, @p;
}

# print Dumper \@points;

foreach (1..10333) {
    my $min_x;
    my $min_y;
    my $max_x;
    my $max_y;
    foreach my $p (@points) {
        $min_x = ($min_x and $min_x < $p->[0]) ? $min_x : $p->[0];
        $min_y = ($min_y and $min_y < $p->[1]) ? $min_y : $p->[1];

        $max_x = ($max_x and $max_x > $p->[0]) ? $max_x : $p->[0];
        $max_y = ($max_y and $max_y > $p->[1]) ? $max_y : $p->[1];
    }
    my $area = ($max_x-$min_x) * ($max_y-$min_y);
    # print "map: $min_x; $min_y $max_x; $max_y\n";
    # print "area: $area\n";
    # if (not $min_area or $area < $min_area) {
    #     print "AREA: $area\tTICK: $tick\tX: ", ($max_x-$min_x), "; ", ($max_y-$min_y), "\n";
    #     $min_area = $area;
    # }

    foreach my $p (@points) {
        $p->[0] += $p->[2];
        $p->[1] += $p->[3];
    }

    ++$tick;
}

my $min_x;
my $min_y;
my $max_x;
my $max_y;
foreach my $p (@points) {
    $min_x = ($min_x and $min_x < $p->[0]) ? $min_x : $p->[0];
    $min_y = ($min_y and $min_y < $p->[1]) ? $min_y : $p->[1];

    $max_x = ($max_x and $max_x > $p->[0]) ? $max_x : $p->[0];
    $max_y = ($max_y and $max_y > $p->[1]) ? $max_y : $p->[1];
}
my $area = ($max_x-$min_x) * ($max_y-$min_y);

# base from 0
foreach my $p (@points) {
    $p->[0] -= $min_x;
    $p->[1] -= $min_y;
}

my $base_x = ($max_x-$min_x);
my $base_y = ($max_y-$min_y);

foreach my $yy (0..$base_y) {
    my @a = ((" ") x $base_x);
    foreach my $p (@points) {
        next if ($p->[1] != $yy);
        $a[$p->[0]] = "#";
    }
    print join('', @a), "\n";
}
