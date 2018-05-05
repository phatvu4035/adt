<?php 
global $base_url;
 ?>
<div class="row">
<div class="col-lg-3 col-xs-6">
  <!-- small box -->
  <div class="small-box bg-aqua">
    <div class="inner">
      <h3><?= $data['tongso']; ?></h3>
      <p>Tổng số thiết bị</p>
    </div>
    <div class="icon">
      <i class="fa fa-rss" aria-hidden="true"></i>
    </div>
    <a href="<?= $base_url.'/mac/list'; ?>" class="small-box-footer">Xem chi tiết<i class="fa fa-arrow-circle-right"></i></a>
  </div>
</div>

<!-- ./col -->
<div class="col-lg-3 col-xs-6">
  <!-- small box -->
  <div class="small-box bg-yellow">
    <div class="inner">
      <h3><?= $data['dakichhoat']; ?></h3>

      <p>Thiết bị kích hoạt</p>
    </div>
    <div class="icon">
      <i class="fa fa-sitemap" aria-hidden="true"></i>
    </div>
    <a href="<?= $base_url.'/mac/list?tinhtrang=1'; ?>" class="small-box-footer">Xem chi tiết <i class="fa fa-arrow-circle-right"></i></a>
  </div>
</div>
<!-- ./col -->

<!-- ./col -->
<div class="col-lg-3 col-xs-6">
  <!-- small box -->
  <div class="small-box bg-green">
    <div class="inner">
      <h3><?= $data['online']; ?></h3>
      <p>Thiết bị online</p>
    </div>
    <div class="icon">
      <i class="fa fa-check" aria-hidden="true"></i>
    </div>
    <a href="<?= $base_url.'/mac/list?trangthai=1'; ?>" class="small-box-footer">Xem chi tiết <i class="fa fa-arrow-circle-right"></i></a>
  </div>
</div>

<!-- ./col -->
<div class="col-lg-3 col-xs-6">
  <!-- small box -->
  <div class="small-box bg-red">
    <div class="inner">
      <h3><?= $data['offline']; ?></h3>

      <p>Thiết bị offline</p>
    </div>
    <div class="icon">
      <i class="fa fa-power-off" aria-hidden="true"></i>
    </div>
    <a href="<?= $base_url.'/mac/list?trangthai=2'; ?>" class="small-box-footer">Xem chi tiết <i class="fa fa-arrow-circle-right"></i></a>
  </div>
</div>
<!-- ./col -->

</div>
