#!/usr/bin/perl
use warnings;
use strict;

sub char2bin {
    my $c = shift @_;
    return 1 << (ord($c) - ord('a'));
}

sub toBin {
    my $input = shift @_;
    my $result = 0;
    foreach my $c (split(//, $input)) {
        $result += char2bin($c);
    }
    return $result;
}

# sub binCmp {
#     my ($lhs, $rhs) = @_;
#     my $lbin = toBin($lhs);
#     my $rbin = toBin($rhs);
#     return $lbin^$rbin;
# }

sub count1 {
    my $in = shift @_;
    my $result = 0;
    while($in > 0) {
        if ($in & 1) {
            $result++;
        }
        $in >>= 1;
    }
    return $result;
}

my @input = ();

while(<>) {
    chomp;
    push(@input, $_);
}

foreach my $i ( 0 .. $#input ) {
    my $lhs = toBin($input[$i]);
    foreach my $j ( $i+1 .. $#input ) {
        my $rhs = toBin($input[$j]);
        if (count1($lhs^$rhs) == 2)  {
            printf("MATCH: [%s] [%s]\n", $input[$i], $input[$j]);
        }
    }
}
