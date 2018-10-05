#!/usr/bin/perl

use strict;
use warnings;

#read file and write a line then n blank lines with seperators only;
# I HATE PERL PROGRAMMING!!!!
# (JUST A BIT :-)  )

my $seperator_char = " ";
my $newLine_char = "\\\n";
my $fileName = $ARGV[0];

#my $outFileNameFlag = ".txt";
#my @outFileNameArr = split(/\./, $fileName);
#print $fileName . "\n";
#print @outFileNameArr . "\n";
my $outFileName = "dukeConv.txt";
open INFILE, "<", $fileName or die "cannot open file";

#my @matrix;
#my $counter = 0;
my $currentLine;
my @curArr;

my $maxSize = 0;

open OUTFILE, ">", $outFileName or die "cannot open";

while(<INFILE>){
    $currentLine = $_;
    chomp($currentLine);    
    @curArr = split(/\s/, $currentLine);
    for(my $i=1; $i <= 8; $i++){
        print OUTFILE $curArr[$i];
        print OUTFILE $seperator_char;
    }
    
    #print OUTFILE "// 0x";
    #print OUTFILE "$curArr[0]";
    print OUTFILE $newLine_char;
}
    
close(INFILE);
close(OUTFILE);
