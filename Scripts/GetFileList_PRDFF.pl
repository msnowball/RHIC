#!/usr/local/bin/perl

use DBI;

###########SETTINGS############
$RUN="PP510Run12";
$MINRUN=364808;
$HPSSDIR="/home/phnxsink/run12/eventdata/";
$DOWNLOADDIR=$ARGV[1];
$MINEVENTSPERFILE=100000;
###############################


###########################
chomp($USER=`whoami`);
push(@machines,"rcas2070");
push(@machines,"rcas2071");
push(@machines,"rcas2072");
push(@machines,"rcas2073");
push(@machines,"rcas2074");
push(@machines,"rcas2075");
push(@machines,"rcas2076");
push(@machines,"rcas2077");
push(@machines,"rcas2078");
###########################


$dbh = DBI->connect("dbi:ODBC:daq") || die $DBI::error;
$dbh->{LongReadLen}=2000; # full file paths need to fit in here

push(@filetype,"PRDFF");

if ( $#ARGV + 1 < 2 ||  $#ARGV + 1 > 2 ) 
{
    print "usage: GetFileList_PRDFF.pl <RunNumber or List.txt> </path/to/download/file/to> \n" ;
    print "possible run numbers from RUN='$RUN',RUNTYPE=PHYSICS with more than 30K events \n";
    $getrunnumbers = $dbh->prepare("select run.runnumber from run, trigger where run.partitionname='Big' and run.triggerconfig like '$RUN' and run.runtype='PHYSICS' and run.loggingon!='NO' and run.eventsinrun>30000 and run.runnumber>='$MINRUN' and run.runnumber=trigger.runnumber and (trigger.name='ZDCLL1wide' or trigger.bitnb=0)");
    $getrunnumbers->execute();
    while(@rn = $getrunnumbers->fetchrow_array())
    {
        print "$rn[0]\n";
    }
    $getrunnumbers->finish();
    exit (-2);
}



if ($ARGV[0] =~ m/.txt/)
{
    open(DATA,'<',$ARGV[0]) or die "Could not open file '$ARGV[0]' $!";
    while (<DATA>){
	if ($_ != "")
	{
	    chomp;
	    push(@runList,$_);
	}
    }
}
else { push(@runList,$ARGV[0]); }
    

$getFiles = $dbh->prepare("select * from prdflist where runnumber = ? order by sequence");

foreach (@runList)
{
    $listfile='HPSSGetPRDFF_run'."$_".'.'.'list';
    $highRun = (int($_/1000+1)*1000);
    $lowRun = $highRun - 1000;
    print "Writing output for run '$_' to '$listfile' \n";
    open(F,">$listfile");

    $getFiles->execute($_);
    
    $counter=0;
    if ($getFiles->rows > 0)
    {

	#Get Column Headers
	#$fields = join(',',@{ $getFiles->{NAME_lc} } );
	#print "$fields\n";
	#filename,runnumber,sequence,status,comment,ctime,size,mtime,events,md5sum,bufferbox,transferred_bytes

	while(@file = $getFiles->fetchrow_array())
	{
	    $theFile=$file[0];
	    $sequence=$file[1];
	    $status=$file[2];
	    $events=$file[8];
	    
	    if($sequence > 0 && $status >= 2 && $events > $MINEVENTSPERFILE)
	    { 
	        if($counter >= @machines){ $counter = 0;}
		$theString1=join('',$HPSSDIR,'/run_0000',$lowRun,'_0000',$highRun,'/',$theFile);
		$theString2= join('','pftp://',$USER,'@',$machines[$counter++],'/',$DOWNLOADDIR,'/',$theFile);
		print F "$theString1 $theString2 \n";
	    }
	    
	}
    }
    else
    {
	print STDERR "No files for run '$_'\n";
    }

    $getFiles->finish();
    close(F);
}

$dbh->disconnect;


