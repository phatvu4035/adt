/*
Version : 1.0
Author: Vikram Salunke
Description: It calculates width of Saved Connections form on page load and apply max width to it to avoid horizontal scroll on the page  
*/
jQuery(document).ready(function() {
	var inn_width = jQuery('#file-transfer-wrapper-config-form').width();
	jQuery('#edit-saved-connection .fieldset-wrapper').css('overflow','scroll').css('max-width', inn_width);
});
