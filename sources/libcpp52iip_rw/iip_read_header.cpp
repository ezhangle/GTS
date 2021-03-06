#include <assert.h>	/* assert() */
#include <string.h>	/* strcmp() */

#include "ptbl_returncode.h"
#include "pri.h"

#include "tif_image_rw.h"
#include "tga_image_rw.h"

#include "iip_read.h"

int iip_read::header( void )
{
	char	*cp_ext;
	int	i_ret;
	long	l_w, l_h, l_c, l_b;
	long	l_tif_bits_per_sample = 0;

	/* ファイルパスの設定はしておかなければならない */
	assert( NULL != this->cl_name.get_cp_name() );

	/* 拡張子を得る */
	cp_ext = strrchr( this->cl_name.get_cp_name(), (int)'.' );
	if (NULL == cp_ext) {
		pri_funct_err_bttvr(
			"Error : %s has not extention.",
			this->cl_name.get_cp_name() );
		return NG;
	}
	/* ".rgb" --> "rgb" */
	++cp_ext;

	/* TIFF画像 */
	if ( !strcmp( "tif", cp_ext )
	||        !strcmp( "TIF", cp_ext )
	) {
		i_ret = tif_image_read_header(
			this->cl_name.get_cp_name(),
			&l_w, &l_h, &l_c, &l_b,
			&(l_tif_bits_per_sample),
			&(this->_d_tif_dpi_x),
			&(this->_d_tif_dpi_y),
			&(this->_l_tif_tile_width),
			&(this->_l_tif_tile_height),
			&(this->_l_tif_compression),
			&(this->_l_tif_orientation)
		);
		if (OK != i_ret) {
			pri_funct_err_bttvr(
	       "Error : tif_image_read_header(-) returns NG." );
			return NG;
		}

	}
	/* Targa画像 */
	else if ( !strcmp( "tga", cp_ext )
	||        !strcmp( "TGA", cp_ext ) ) {
		i_ret = tga_image_read_header(
			this->cl_name.get_cp_name(),
			&l_w, &l_h, &l_c, &l_b
		);
		if (OK != i_ret) {
			pri_funct_err_bttvr(
	       "Error : tga_image_read_header(-) returns NG." );
			return NG;
		}
	}
	else {
		pri_funct_err_bttvr(
		"Error : not support extention<%s>.\n", cp_ext );
		return NG;
	}

	this->set_l_width(    l_w );
	this->set_l_height(   l_h );
	this->set_l_channels( l_c );

	/* チャンネル毎のセット */
	switch (l_b) {
	case sizeof(unsigned char):
		this->cl_ch_info.set_e_ch_num_type(E_CH_NUM_UCHAR);
		break;
	case sizeof(unsigned short):
		this->cl_ch_info.set_e_ch_num_type(E_CH_NUM_USHRT);
		break;
	case 0:
		if (1L == l_tif_bits_per_sample) {
		 this->cl_ch_info.set_e_ch_num_type(E_CH_NUM_BITBW);
		} else {
		 pri_funct_err_bttvr(
			"Error : bad l_bytes<%ld> and bits<%ld>",
			l_b, l_tif_bits_per_sample );
		 return NG;
		}
		break;
	default:
		pri_funct_err_bttvr( "Error : bad l_bytes<%ld>", l_b );
		return NG;
	}

	return OK;
}

