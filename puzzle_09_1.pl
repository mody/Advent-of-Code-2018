#!/usr/bin/perl
use warnings;
use strict;

sub clockwise {
    my ($a, $pos) = @_;
    ++$pos;
    if ($pos == scalar @$a) {
        $pos = 0;
    }
    return $pos;
}

sub counter_clockwise {
    my ($a, $pos, $step) = @_;
    $step //= 1;
    for (; $step > 0; --$step) {
        if ($pos == 0) {
            $pos = scalar @$a;
        }
        --$pos;
    }
    return $pos;
}

sub insert_after {
    my ($a, $pos, $val) = @_;
    ++$pos;
    if ($pos == scalar @$a) {
        push @$a, $val;
        return scalar @$a - 1;
    }
    splice(@$a, $pos, 0, $val);
    return $pos;
}

sub remove_at {
    my ($a, $pos) = @_;
    return scalar splice(@$a, $pos, 1);
}


my @pole = (0, 1);
my @players;
my $player = 2;
my $current = 1;

my $max_players = 425;
my $max_balls = 70848;

foreach my $num (2..$max_balls) {
    if ($num % 23 == 0) {
        $players[$player] //= 0;
        $current = counter_clockwise(\@pole, $current, 7);
        $players[$player] += ($num + remove_at(\@pole, $current));
    } else {
        my $after = clockwise(\@pole, $current);
        $current = insert_after(\@pole, $after, $num);
    }
    ++$player;
    if ($player > $max_players) {
        $player = 1;
    }
}

@players = sort @players;
print $players[-1], "\n";
