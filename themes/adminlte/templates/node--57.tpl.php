<?php 
$conn = ftp_connect('ftp.vintest.websumo.vn',2157,180);

if(ftp_login($conn,'vuphat','RRnrReSGwLr7m7pL76TRZwCXKg')) {

	ftp_pasv($conn,FALSE);
    ftp_chdir($conn,'files');
	$path = ftp_pwd($conn);

	$pre = rand(10,100);
	$name = $pre.'ahbp.jpg';

	$fP = 'sites/default/files/video_upload_content/hsts.jpg';

	$pss = ftp_put($conn,$path.'/'.$name,$fP,FTP_BINARY);
	var_dump($pss);

	if(!$pss) {
		$psr = ftp_put($conn,$path.'/'.$name,$fP,FTP_ASCII);
		var_dump($psr);
	} 
} 

 ?>
