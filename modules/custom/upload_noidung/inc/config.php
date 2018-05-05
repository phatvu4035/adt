<?php 
function config_upload_noidung() {
 	return $config = array(
 		'ftp_host' => '10.100.8.61',
 		'user' => 'nginx',
 		'password' => 'Vingroup@123',
 		'port' => 22,
 		'path_to_save' => '/var/www/html/vod/',
 		'domain_open' => 'http://cdnmkt.vingroup.net/vod/',
 	);
}