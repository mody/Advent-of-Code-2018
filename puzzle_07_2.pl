#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

sub to_time {
    my ($c) = @_;
    return ord($c) - ord('A') + 1;
}


sub get_root {
    my ($h) = @_;

    my @deps = map {@$_} values %$h;

    my @w;

    foreach my $k (sort keys %$h) {
        my $l = scalar grep { $k eq $_} @deps;
        if ($l == 0) {
            push @w, ($k);
        }
    }
    return @w;
}

sub make_done {
    my ($h, $w) = @_;

    delete $$h{$w};

    while (my ($k, $v) = each %$h) {
        @$v = grep {$w ne $_} @$v;
        foreach my $i (@$v) {
            if (not exists $$h{$i}) {
                $$h{$i} = [];
                scalar keys %$h; # reset irerator
            }
        }
    }

}


my %h;

# Step C must be finished before step A can begin.
while(<>) {
    chomp;
    my @a = split(/ /);
    my ($x, $y) = ($a[1], $a[7]);
    push @{$h{$x}}, $y;
}

print Dumper \%h;

my %workers;
my $nworkers = 5;

my @queue;
my $n;
my $time = 0;

print "==============\n";
map { push @queue, $_ } get_root(\%h);
# print Dumper \@queue;

while(@queue > 0 or %workers > 0) {
    while ((scalar %workers) < $nworkers) {
        my $n = shift @queue;
        # print Dumper $n;
        last if not $n;
        $workers{$n} = ord($n) - ord('A') + 61;
    }
    # make tick

    print "TICK: $time, WORKERS: ", scalar %workers, ", QUEUE: ", scalar @queue, "\n", Dumper \%workers;

    my @todo;
    foreach my $k (keys %workers) {
        my $v = \$workers{$k};
        --$$v;
        if ($$v == 0) {
            # return if not exists $h{$n};k
            map { push @queue, $_ } @{$h{$k}};
            make_done(\%h, $k);
            push @todo, $k;
        }
    }
    foreach my $k (@todo) {
        delete $workers{$k};
    }

    ++$time;
    print "----------------\n";
}

print "TIME=$time\n";
