
<div class="row">
	<div class="col-md-5">
		<?php print render($form['name_playlist']); ?>
	</div>
</div>
<div>
	<?php print render($form['find_content']); ?>
	<?php print render($form['show_grid_content']); ?>
</div>
<div>
	<div class="col-md-10">
		
	</div>
	<div class="col-md-2">
		<?php print render($form['submit']); ?>
	</div>
</div>
<div>	
	<?php print drupal_render_children($form); ?>
</div>