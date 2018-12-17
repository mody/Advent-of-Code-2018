#!/usr/bin/perl
use warnings;
use strict;

use Data::Dumper;

# [1518-02-18 23:59] Guard #2141 begins shift
# [1518-02-19 00:24] falls asleep
# [1518-02-19 00:58] wakes up
# [1518-02-20 00:01] Guard #2777 begins shift
# [1518-02-20 00:16] falls asleep
# [1518-02-20 00:20] wakes up
# [1518-02-20 00:46] falls asleep
# [1518-02-20 00:53] wakes up
# [1518-02-21 00:00] Guard #2749 begins shift
# [1518-02-21 00:09] falls asleep
# [1518-02-21 00:49] wakes up
# [1518-02-21 23:58] Guard #2749 begins shift
# [1518-02-22 00:48] falls asleep
# [1518-02-22 00:59] wakes up
# [1518-02-23 00:01] Guard #2749 begins shift


my %g;
my $line = 0;

my $guardId;
my $fromM;
my $toM;

while(<>) {
    $line++;
    chomp;
    if (not m/^\[\d+-\d+-\d+ (\d+):(\d+)\] (.*)$/) {
        die "invalid input!";
    }
    my ($hh, $mm, $op) = ($1, $2, $3);

    # state machine
    if ($op =~ /^Guard #(\d+)/) {
        # expect "Guard #2749 begins shift"
        $guardId = $1;
        $fromM = $toM = undef;

        if (not exists $g{$guardId}) {
            foreach my $i (0..59) {
                $g{$guardId}[$i] = 0;
            }
        }
        next;
    } elsif ($op eq "falls asleep") {
        $fromM = $mm;
        $toM = undef;
        next;
    } elsif ($op eq "wakes up") {
        $toM = $mm;
    } else {
        die("invalid state at line $line: ".$op);
    }

    if (not ($guardId and $fromM and $toM) or $fromM >= $toM) {
        die("consistency error at line $line");
    }

#    print "Guard $guardId sleeps from $hh:$fromM to $hh:$toM\n";

    foreach my $i ($fromM..($toM-1)) {
        $g{$guardId}[$i] += 1;
    }
}

#while ( my ($key, $value) = each %g ) {
#    print $key, "\n";
#    while ( my ($index, $value) = each @{$value} ) {
#        print "\t", $index, ": ", ($value or ""), "\n";
#    }
#}

my $sumM = -1;

while ( my ($key, $value) = each %g ) {
    my $sum = 0;
    while ( my ($index, $value) = each @{$value} ) {
        $sum += $value;
    }
    if ($sumM < $sum) {
        $sumM = $sum;
        $guardId = $key;
    }
}

my $maxM = -1;
my $max = -1;

while ( my ($index, $value) = each @{$g{$guardId}} ) {
    if ($max < $value) {
        $max = $value;
        $maxM = $index;
    }
}

print "GUARD: $guardId\nMAX: $max\nMAX_M: $maxM\nSUM: $sumM\nRESULT: ",$guardId*$maxM,"\n";
