#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;
use POSIX;

my @a;
my %rules;
my %h;
my $dup;

my $line = <>;
chomp $line;
@a = split(//, substr($line, 15));
$h{substr($line, 15)} = 0;

# skip empty line
$line = <>;

while (<>) {
    chomp;
    die "invalid rule [$_]" if not m/([.#]{5}) => (.)$/;
    $rules{$1} = $2;
}

my ($pre, $post) = (5, 5);
foreach (1..$pre) {
    unshift(@a, '.');
}
foreach (1..$post) {
    push(@a, '.');
}

print "0: ", join('', @a), "\n";

# foreach my $gen (1..500) {
# foreach my $gen (1..50000000000) {
foreach my $gen (1..20) {

    my $line = join('', @a);

    # my $n = $h{$line};
    # if (defined $n) {
    #     print "$gen: ", join('', @a), "\n";
    #     print "DUP in gen $gen points to gen $n\n";
    #     $dup = $gen;
    #     last;
    # }

    foreach my $from (0..(scalar @a - 5)) {
        my $s = substr($line, $from, 5);
        my $r = $rules{$s};
        if (not defined $r) {
            $r = '.';
            # print "undefined rule [$s]\n";
        }
        $a[$from+2] = $r;
    }

    # strip leading spaces
    while(1) {
        last if not ($a[0] eq '.' and $a[1] eq '.' and $a[2] eq '.' and $a[3] eq '.' and $a[4] eq '.');
        shift(@a);
        --$pre;
    }
    while(1) {
        last if not ($a[-1] eq '.' and $a[-2] eq '.' and $a[-3] eq '.' and $a[-4] eq '.' and $a[-5] eq '.');
        pop(@a);
        --$post;
    }

    # extend
    while(1) {
        last if ($a[0] eq '.' and $a[1] eq '.' and $a[2] eq '.' and $a[3] eq '.' and $a[4] eq '.');
        unshift(@a, '.');
        ++$pre;
    }
    while(1) {
        last if ($a[-1] eq '.' and $a[-2] eq '.' and $a[-3] eq '.' and $a[-4] eq '.' and $a[-5] eq '.');
        push(@a, '.');
        ++$post;
    }

    # $h{$line} = $gen;

    my $sum = 0;
    while (my ($i, $c) =  each @a) {
        if ($c eq '#') {
            $sum += $i - $pre;
        }
    }

    # print "$gen; sum: $sum, pre: $pre, post: $post\n";
    # print "$gen: ", join('', @a), "; sum: $sum, pre: $pre, post: $post\n";
    # print $gen, " - ", ($gen*100/50000000000), " - ", scalar @a, "\n" if $gen % 1000 == 0;
}

# print ("DUP@ $dup\nremainder: ", POSIX::fmod(50000000000, $dup), "\n");

$line = ".....##..##...##.....##...##.....##..##...##...##...##..##...##...##..##..##..##...##..##...##..##..##...##..##...##..##..##..##...##..##..##...##..##..##...##...##...##...##...##..##..##.....";
@a = split(//, $line);

$pre = -1 *((50000000000-100)+13);

my $sum = 0;
while (my ($i, $c) =  each @a) {
    if ($c eq '#') {
        $sum += $i - $pre;
    }
}

print "SUM: $sum\n";
