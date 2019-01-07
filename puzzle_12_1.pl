#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

my @a;
my %rules;

my $line = <>;
chomp $line;
@a = split(//, substr($line, 15));

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

# print "  0: ", join('', @a), "\n";

foreach my $gen (1..20) {

    # my @b;
    # push(@b, '.');
    # push(@b, '.');

    my $line = join('', @a);

    foreach my $from (0..(scalar @a - 5)) {
        my $s = join('', @a[$from..($from+4)]);
        my $r = $rules{$s};
        if (not defined $r) {
            $r = '.';
            # print "undefined rule [$s]\n";
        }
        push(@b, $r);
    }
    push(@b, '.');
    push(@b, '.');

    # strip leading spaces
    while(1) {
        last if ($b[0] eq '.' and $b[1] eq '.' and $b[2] eq '.' and $b[3] eq '.' and $b[4] eq '.');
        unshift(@b, '.');
        ++$pre;
    }

    while(1) {
        last if ($b[-1] eq '.' and $b[-2] eq '.' and $b[-3] eq '.' and $b[-4] eq '.' and $b[-5] eq '.');
        push(@b, '.');
        ++$post;
    }

    @a = @b;
    # print "$gen: ", join('', @a), "\n";
    print "$gen - ", scalar @a, "\n" if $gen % 100 == 0;
}

my $sum = 0;
while (my ($i, $c) =  each @a) {
    if ($c eq '#') {
        $sum += $i - $pre;
    }
}

print "SUM: $sum\n";
