<?php
/*
* Form hien thi danh sach MAC
*/
function manage_mac_by_list() {
   $index = 1;
   $terms = entity_load('taxonomy_term',FALSE,array('vid' => 5));
    if(!isset($terms)) {
        $terms = array();
    }
?>
<?php ob_start(); ?>
<div class="page_update_mac">
	<div>
		<div class="box-update-mac">
			<a href="update-mac"><span class="label label-success">Update địa chỉ MAC</span></a>
		</div>
	</div>
    <table class="table table-striped">
        <thead>
            <tr>
                <th>STT</th>
                <th>Đại chỉ MAC</th>
                <th>Ghi chú</th>
                <th>Ngày nhập kho</th>
                <th>Trạng thái</th>
                <th>Cửa hàng</th>
                <th>Tỉnh thành</th>
                <th>Khu vực</th>
                <th>Công ty</th>
                <th>Thao tác</th>
            </tr>
        </thead>
        <tbody>
      <?php foreach($terms as $term): ?>
      	    <?php 
      	    	if(isset($term->field_mac_store_id['und'][0]['tid']) && !empty($term->field_mac_store_id['und'][0]['tid'])) {
      	    		$store_id = $term->field_mac_store_id['und'][0]['tid'];
      	    		$store = taxonomy_term_load($store_id);
      	    	}
      	    	if(isset($term->field_mac_province_id['und'][0]['tid']) && !empty($term->field_mac_province_id['und'][0]['tid'])) {
      	    		$province_id = $term->field_mac_province_id['und'][0]['tid'];
      	    		$province = taxonomy_term_load($province_id);
      	    	}
      	    	if(isset($term->field_mac_region_id['und'][0]['tid']) && !empty($term->field_mac_region_id['und'][0]['tid'])) {
      	    		$region_id = $term->field_mac_region_id['und'][0]['tid'];
      	    		$region = taxonomy_term_load($region_id);
      	    	}
      	    	if(isset($term->field_mac_company_id['und'][0]['tid']) && !empty($term->field_mac_company_id['und'][0]['tid'])) {
      	    		$company_id = $term->field_mac_company_id['und'][0]['tid'];
      	    		$company = taxonomy_term_load($company_id);
      	    	}
              if(isset($term->field_device_status['und'][0]['value']) && !empty($term->field_device_status['und'][0]['value'])) {
                 $status_id = $term->field_device_status['und'][0]['value'];
              }
              if(isset($term->field_imported_date['und'][0]['value']) && !empty($term->field_imported_date['und'][0]['value'])) {
                 $date = $term->field_imported_date['und'][0]['value'];
              }
      	    ?>

            <tr>
                <td><?= $index; ?></td>
                <td><?= $term->name; ?></td>
                <td><?= $term->description; ?></td>
                <td>
                   <?= isset($date) ? $date : ''; ?>
                </td>
                <td>
                  <?php 
                    if(isset($status_id) && $status_id == 1) {
                       echo '<span class="label label-success">Đang kích hoạt</span>';
                    }
                    if(isset($status_id) && $status_id == 2) {
                       echo '<span class="label label-success">Đã khóa</span>';
                    }
                   ?>
                </td>
                <td><?= (isset($store)) ? $store->name : ''; ?></td>
                <td><?= (isset($province)) ? $province->name : ''; ?></td>
                <td><?= (isset($region)) ? $region->name : ''; ?></td>
                <td><?= (isset($company)) ? $company->name : ''; ?></td>
                <td>
                    <ul class="tools">
                    <li><a href="<?php echo $GLOBALS['base_url']; ?>/taxonomy/term/<?= $term->tid; ?>/delete" class="btn btn-danger"><i class="fa fa-remove"></i> Xóa</a></li>
                </ul>
                </td>
            </tr>
      <?php $index++; ?>
      <?php endforeach; ?>
        </tbody>
    </table>
</div>

<?php
  $markup = ob_get_clean();
  return ['#markup' => $markup];
  // Het phan render company
} 
?>