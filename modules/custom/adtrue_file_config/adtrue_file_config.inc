<?php

// Form cau hinh
function file_transfer_config_server($form,&$form_state) {
	$form = array();
	// Get value default
	$server_ip = variable_get('adtrue_file_server_ip');
	$server_user = variable_get('adtrue_file_server_user');
	$server_pass = variable_get('adtrue_file_server_pass');
	$server_port = variable_get('adtrue_file_server_port');
	$server_path_save = variable_get('adtrue_file_server_path_save');
	$server_domain_open = variable_get('adtrue_file_server_domain_open');

	$form['server_config'] = array(
		'#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
	);
	$form['server_config']['header'] = array(
		'#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
	);
	$form['server_config']['body'] = array(
		'#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
	);
	$form['server_config']['footer'] = array(
		'#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
	);
	$form['server_config']['header']['header_title'] = array(
		'#type' => 'item',
        '#markup' => '<h3>Cấu hình SFTP</h3>'
	);
	if($server_ip) {
		$form['server_config']['header']['header_info_config'] = array(
			'#type' => 'item',
			'#markup' => '<div class="col-md-6">
			      			<p><strong>Địa chỉ server: </strong>'.$server_ip.'</p>
			      			<p><strong>Tên đăng nhập: </strong>'.$server_user.'</p>
			      			<p><strong>Port: </strong>'.$server_port.'</p>
					    </div>
					    <div class="col-md-6">
					    	<p><strong>Đường dẫn lưu file trên host: </strong>'.$server_path_save.'</p>
			      			<p><strong>Tên miền mở file: </strong>'.$server_domain_open.'</p>
					    </div>'
		);
	}
	$form['server_config']['body']['col-left']  = array(
		'#type' => 'container',
        '#weight' => 1,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>',
	);
	$form['server_config']['body']['col-right']  = array(
		'#type' => 'container',
        '#weight' => 1,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>',
	);
	$form['server_config']['body']['col-left']['server_ip'] = array(
		'#type' => 'textfield',
        '#title' => t('Địa chỉ server'),
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
        '#default_value' => $server_ip,
	);

	$form['server_config']['body']['col-left']['server_user'] = array(
		'#type' => 'textfield',
        '#title' => t('Tên đăng nhập'),
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
        '#default_value' => $server_user,
	);
	$form['server_config']['body']['col-left']['server_pass'] = array(
		'#type' => 'password', 
  		'#title' => t('Password'), 
  		'#maxlength' => 100, 
  		'#size' => 30,
  		'#required' => TRUE,
        '#default_value' => $server_pass,
	);
	$form['server_config']['body']['col-right']['server_port'] = array(
		'#type' => 'textfield',
        '#title' => t('Port'),
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
        '#default_value' => $server_port,
	);
	$form['server_config']['body']['col-right']['server_path_save'] = array(
		'#type' => 'textfield',
        '#title' => t('Đường dẫn lưu file trên host'),
        '#size' => 100,
        '#maxlength' => 128,
        '#required' => TRUE,
        '#default_value' => $server_path_save,
	);
	$form['server_config']['body']['col-right']['server_domain_open'] = array(
		'#type' => 'textfield',
        '#title' => t('Tên miền mở file'),
        '#size' => 100,
        '#maxlength' => 128,
        '#required' => TRUE,
        '#default_value' => $server_domain_open,
	);
	$form['server_config']['footer']['server_submit'] = array(
		'#type' => 'submit',
        '#value' => 'Lưu lại',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-success')
        ),
	);

	return $form;
}

// Validate
function file_transfer_config_server_validate($form,&$form_state) {
	
}

function file_transfer_config_show($form,&$form_state) {
    global $base_url;
	$header = array(
        array('data' => 'STT'),
        array('data' => 'Địa chỉ Server'),
        array('data' => 'Tên đăng nhập'),
        array('data' => 'Port'),
        array('data' => 'Tên miền'),
        array('data' => 'Đường dẫn trên host'),
        array('data' => 'Thao tác'),
    );
    $rows = fetch_file_transfer_config();
    $form['file_transfer'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['file_transfer']['header'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['file_transfer']['header']['create_photo'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/transfer_file/config/setting" class="btn btn-success pull-right"><i class="fa fa-plus"></i>Cài đặt cấu hình</a>',
    );
    $form['file_transfer']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Sử dụng cấu hình mặc định',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['file_transfer']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}

function fetch_file_transfer_config() {
	global $base_url;
	$server_ip = variable_get('adtrue_file_server_ip','');
	$server_user = variable_get('adtrue_file_server_user','');
	$server_port = variable_get('adtrue_file_server_port','');
	$server_path_save = variable_get('adtrue_file_server_path_save','');
	$server_domain_open = variable_get('adtrue_file_server_domain_open','');
	$rows = array();
	if(!$server_ip) {
		return $rows;
	} else {
		$rows[] = array(
			1,
			$server_ip,
			$server_user,
			$server_port,
			$server_domain_open,
			$server_path_save,
			'<div class="btn-group">
	            <button type="button" class="btn btn-success">Thao tác</button>
	            <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
	                <span class="caret"></span>
	                <span class="sr-only">Toggle Dropdown</span>
	            </button>
	            <ul class="dropdown-menu" role="menu">
	                <li><a href="'.$base_url.'/transfer_file/config/setting" ><i class="fa fa-edit"></i> Sửa</a></li>
	                <li><a href="'.$base_url.'/transfer_file/config/delete/" ><i class="fa fa-trash"></i> Xóa</a></li>
	            </ul>
          </div>'
		);
		return $rows;
	}
}

function file_transfer_config_delete($form,&$form_state) {
	global $base_url;

	$server_ip = variable_get('adtrue_file_server_ip','');
	$server_user = variable_get('adtrue_file_server_user','');
	$server_port = variable_get('adtrue_file_server_port','');
	$server_path_save = variable_get('adtrue_file_server_path_save','');
	$server_domain_open = variable_get('adtrue_file_server_domain_open','');

	$form['file_transfer'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-danger')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['file_transfer']['header'] = array(
        '#type' => 'item',
        '#markup' =>'<h3 class="col-md-10">Bạn có muốn xóa cấu hình ? </h3> <a href="'.$base_url.'/transfer_file/config/statistic" class="btn btn-info pull-right" style="margin-top:24px;"><i class="fa fa-reply"></i> Quay lại </a>',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['file_transfer']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['file_transfer']['body']['header_info_config'] = array(
		'#type' => 'item',
		'#markup' => '<div class="col-md-6">
  			<p><strong>Địa chỉ server: </strong>'.$server_ip.'</p>
  			<p><strong>Tên đăng nhập: </strong>'.$server_user.'</p>
  			<p><strong>Port: </strong>'.$server_port.'</p>
	    </div>
	    <div class="col-md-6">
	    	<p><strong>Đường dẫn lưu file trên host: </strong>'.$server_path_save.'</p>
  			<p><strong>Tên miền mở file: </strong>'.$server_domain_open.'</p>
	    </div>'
	);
	 $form['file_transfer']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-footer width-border">',
        '#suffix' => '</div>'
    );

    $form['file_transfer']['footer']['delete_config'] = array(
        '#type' => 'submit',
        '#value' => 'Xóa cấu hình',
        '#attributes' => array(
            'class' => array('btn','btn-danger')
        ),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>'
    );
    return $form;
}
function file_transfer_config_delete_submit($form,&$form_state) {
    global $base_url;
    variable_del('adtrue_file_server_ip');
    variable_del('adtrue_file_server_user');
    variable_del('adtrue_file_server_pass');
    variable_del('adtrue_file_server_port');
    variable_del('adtrue_file_server_path_save');
    variable_del('adtrue_file_server_domain_open');
    drupal_set_message('Đã xóa cấu hình và sử dụng cấu hình mặc định');
    $form_state['redirect'] = $base_url.'/'.'transfer_file/config/statistic';
}
//Submit
function file_transfer_config_server_submit($form,&$form_state) {
    global $base_url;
	$server_ip = $form_state['values']['server_ip'];
	$server_user = $form_state['values']['server_user'];
	$server_pass = $form_state['values']['server_pass'];
	$server_port = $form_state['values']['server_port'];
	$server_path_save = $form_state['values']['server_path_save'];
	$server_domain_open = $form_state['values']['server_domain_open'];

	variable_set('adtrue_file_server_ip',$server_ip);
	variable_set('adtrue_file_server_user',$server_user);
	variable_set('adtrue_file_server_pass',$server_pass);
	variable_set('adtrue_file_server_port',$server_port);
	variable_set('adtrue_file_server_path_save',$server_path_save);
	variable_set('adtrue_file_server_domain_open',$server_domain_open);

	drupal_set_message('Đã lưu cấu hình SFTP');
    $form_state['redirect'] = $base_url.'/'.'transfer_file/config/statistic';
}