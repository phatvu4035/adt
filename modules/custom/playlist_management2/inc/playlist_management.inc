<?php
function list_playlist_job($form,&$form_state) {

	$result = get_list_playlist();

	$rows = array();
	$form['list_playlist'] = array(
		'#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success','wrapper-form')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
	);
	$form['list_playlist']['header'] = array(
		'#type' => 'item',
        '#markup' =>'<a href="'.$GLOBALS['base_url'].'/playlist/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i>Thêm playlist</a>',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
	);

	if($result->rowCount()) {
		foreach($result as $row) {
			$node = node_load($row->nid);
			$check['appr'] = '';
			$form['list_playlist']['playlist_name'][$row->nid] = array(
				'#type' => 'item',
				'#markup' =>'<a href="'.$GLOBALS['base_url'].'/playlist/edit/'.$row->nid.'" >'.$node->title.'</a>',
			);
			$form['list_playlist']['checkboxes_appr'][$row->nid] = array(
				'#type' => 'checkboxes',
				'#options' => $check,
				'#id' => $row->nid,
				'#attributes' =>  array('class' => array('nid-' . $row->nid))
			);
		}
	}
	$form['submit'] = array(
  		'#type' => 'submit',
  		'#weight' => 6,
  		'#value' => t('Cập nhật')
  	);
	$form['list_playlist']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}

function playlist_management_theme() {
	return array(
		'list_playlist_job' => array(
			'render element' => 'form',
		),
	);
}

function theme_list_playlist_job($variables) {
	$form = $variables['form'];
	$index = 1;

	foreach(element_children($form['list_playlist']['playlist_name']) as $key) {
		$row = array();
		$row[] = $index;
		$row[] = array('data' => $form['list_playlist']['playlist_name'][$key],'class' => array('playlist'),'id' =>'titleplaylist-'.$key );
		$row[] = array('data' =>$form['list_playlist']['checkboxes_appr'][$key],'class' => array('check-appr'),'id' => 'check-appr'.$key );
		$node = node_load($key);
		$changed = $node->changed;
		$update = isset($changed) ? date('H:i:s d/m/Y',$changed) : '';
		$row[] = $update;
		$row[] = '<ul class="tools">
                      <li><a href="'.$GLOBALS['base_url'].'/playlist/edit/'.$node->nid.'" class="btn btn-warning"><i class="fa fa-edit"></i>Sửa</a></li>
                      <li><a href="'.$GLOBALS['base_url'].'/playlist//delete/'.$node->nid.'" class="btn btn-danger"><i class="fa fa-remove"></i>Xóa</a></li>
                   </ul>';
		$rows[] = $row;
		$index++;
	}

	$header = array(
		'data' => 'STT',
		'data' => 'Tên playlist',
		'data' => 'Ngày tạo',
		'data' => 'Thao tác'
	);
	
	$output = '';
	$output .= theme('table',array('header' => $header,'rows' => $rows,'attributes' => array('id' => 'table_playlist_list')));
	$output .= drupal_render_children($form);
	return $output;
}

function list_playlist_page_display() {

}

function get_list_playlist() {
	$limit = 2;
	$query = db_select('node','n')->extend('PagerDefault')->distinct()->limit($limit);
	$query->condition('n.type','playlist');

	if(isset($_GET['trangthai']) && (int)$_GET['trangthai'] > 0) {
		$trangthai = $_GET['trangthai'];
		$query->leftJoin('field_data_field_playlist_approval','appr','appr.entity_id = n.nid');
		$query->condition('appr.field_playlist_approval_value',$trangthai);
	}
	if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
		$keyword = $_GET['keyword'];
		$query->condition('n.title','%'.db_like($keyword).'%','LIKE');
	}
	$query->fields('n',array('nid'));
	$query->orderBy('n.changed','DESC');
	$result = $query->execute();
	return $result;

}

function playlist_search_block_form($form,&$form_state) {
	$form['block_playlist']['header_filter'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border" id="header-filter">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('box','box-header')
        ),
    );
    $form['block_playlist']['header_filter']['keyword'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên playlist'),
        '#size' => 60,
        '#default_value' => isset($keyword) ? $keyword : NULL,
        '#attributes' => array(
          'placeholder' => 'Tìm tên playlist',
        ),
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-4 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['block_playlist']['header_filter']['trangthai'] = array(
         '#type' => 'select',
         '#title' => t('Trạng thái'),
         '#options' => array(0=>'Tất cả',1 => 'Được phê duyệt',2=>'Chưa được phê duyệt'),
         '#prefix' => '<div class="col-md-2 box-search-item">',
         '#default_value' => isset($trangthai) ? $trangthai : 0,
         '#suffix' => '</div>',
         '#validated' => TRUE
    );
    $form['block_playlist']['header_filter']['submit_search'] = array(
        '#type' => 'submit',
        '#weight' => 2,
        '#value' => t('Tìm kiếm'),
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['#method'] = 'GET';
    return $form;
}