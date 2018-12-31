#!/usr/bin/perl
use v5.14; use warnings; use List::AllUtils qw<min>; use Getopt::Long;

GetOptions('w=i' => \(my $available_workers = 5), 'd=i' => \(my $min_duration = 60)) or die "Unrecognized options";
my (%depends, %running_until);
while (<>) {
    /Step (.) .* step (.)/g or die "Unrecognized input at line $.: $_";
    $depends{$1} //= '';
    $depends{$2} .= $1;
}
my $time = 0;
while (1) {
    foreach my $step (sort grep { $depends{$_} eq '' } keys %depends) {
        last if !$available_workers;
        $available_workers--;
        push @{$running_until{$time + (ord $step) - (ord 'A') + 1 + $min_duration}}, $step;
        delete $depends{$step};
    }
    last if !%running_until;
    $time = min keys %running_until;
    foreach my $finished (@{delete $running_until{$time}}) {
        print $finished;
        $depends{$_} =~ s/$finished// foreach keys %depends;
        $available_workers++;
    }
}
say " $time";
