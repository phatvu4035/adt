<?php 
$conn = ftp_connect('123.31.12.125',21,90);
if(ftp_login($conn,'vuhien','t7juDKGdlEOYZjGZzGTB')) {
  
	ftp_pasv($conn,TRUE);
    ftp_chdir($conn,'domains/files1');
	$path = ftp_pwd($conn);

	$contents = ftp_nlist($conn, $path);
	ftp_get($conn,'xyp.txt',$path,FTP_ASCII);
	var_dump($contents);

	$teup = ftp_get($conn,'C:\Users\asus\Desktop\producta.txt',$path.'/'.'text1.txt',FTP_BINARY);
	var_dump($teup);
	if(!$teup) {
		$t1 = ftp_get($conn,'C:\Users\asus\Desktop\producta.txt',$path.'/'.'text1.txt',FTP_ASCII);
		var_dump($t1);
	}

} 
 ?>