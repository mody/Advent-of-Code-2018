#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

sub is_cart {
    my ($c) = @_;
    if($c eq 'v' or $c eq '^' or $c eq '>' or $c eq '<') {
        return 1;
    } else {
        return 0;
    }
}

sub dir {
    my ($d) = @_;
    my ($dx, $dy) = (0, 0);
    if ($d eq '<') { $dx = -1; }
    elsif ($d eq '>') { $dx = +1; }
    elsif ($d eq '^') { $dy = -1; }
    elsif ($d eq 'v') { $dy = +1; }
    return ($dx, $dy);
}

sub right {
    my ($d) = @_;
    my %move = (
        '^' => '>',
        '>' => 'v',
        'v' => '<',
        '<' => '^');
    return $move{$d};
}

sub left {
    my ($d) = @_;
    my %move = (
        '>' => '^',
        'v' => '>',
        '<' => 'v',
        '^' => '<');
    return $move{$d};
}


my @plan;

while(<>) {
    chomp;
    push @plan, [split //];
}

my @cart;

while(my ($y, $row) = each @plan) {
    while(my ($x, $c) = each @$row) {
        next if not is_cart($c);
        push @cart, [$x, $y, $c, 0];
        if ($c eq '<' or $c eq '>') {
            $plan[$y][$x] = '-';
        } else {
            $plan[$y][$x] = '|';
        }
    }
}

@cart = sort { $a->[1] <=> $b->[1] || $a->[0] <=> $b->[0] } @cart;


while(1) {
    # print "TICK\n";
    # my @p2 = map { [@$_] } @plan;
    # for my $c (@cart) {
    #     $p2[$c->[1]][$c->[0]] = $c->[2];
    # }
    # while(my ($y, $row) = each @p2) {
    #     while(my ($x, $c) = each @$row) {
    #         print $c;
    #     }
    #     print "\n";
    # }

    if (scalar @cart == 1) {
        print $cart[0][0], ",", $cart[0][1], "\n";
        last;
    }

    my @cart2;
    while( my $c = shift(@cart)) {
        my ($x, $y, $d, $op) = @$c;
        my ($dx, $dy) = dir($d);
        $x += $dx;
        $y += $dy;

        # detect crash
        my $crash = 0;
        for (@cart2) {
            if ($_->[0] == $x and $_->[1] == $y) {
                $crash = 1;
                last;
            }
        }
        if ($crash) {
            @cart2 = grep { !($_->[0] == $x and $_->[1] == $y) } @cart2;
            next;
        }

        $crash = 0;
        for (@cart) {
            if ($_->[0] == $x and $_->[1] == $y) {
                $crash = 1;
                last;
            }
        }
        if ($crash) {
            @cart = grep { !($_->[0] == $x and $_->[1] == $y) } @cart;
            next;
        }

        if ($plan[$y][$x] eq '+') {
            if ($op == 0) {
                $d = left($d);
                $op = 1;
            } elsif ($op == 1) {
                $op = 2;
            } elsif ($op == 2) {
                $d = right($d);
                $op = 0;
            }
        } elsif ($plan[$y][$x] eq '/') {
            # > == left
            # v == right
            if ($d eq '>' or $d eq '<') {
                $d = left($d);
            } else {
                $d = right($d);
            }
        } elsif ($plan[$y][$x] eq '\\') {
            # v == left
            # < = right
            if ($d eq '>' or $d eq '<') {
                $d = right($d);
            } else {
                $d = left($d);
            }
        }
        push(@cart2, [$x, $y, $d, $op]);
    }

    @cart = sort { $a->[1] <=> $b->[1] || $a->[0] <=> $b->[0] } @cart2;
}
