#ifndef __gtsfbro01path_base_h__
#define __gtsfbro01path_base_h__

#include <stdio.h> /* NULL */

#if defined _WIN32
#define	snprintf _snprintf	/* For MS-C v12.00 */
#endif

#include "ptbl_path_max.h" /* PTBL_PATH_MAX(=4096) */
#include "ptbl_returncode.h"
#include "pri.h"

class gtsfbro01path_base {
public:
	gtsfbro01path_base() :
		_ccp_image_file_ext(".tif")
	{
#if defined _WIN32
		strcpy( this->_ca_path   ,"C:/" );
		strcpy( this->_ca_memory ,"C:/" );
#else
		strcpy( this->_ca_path   ,"/" );
		strcpy( this->_ca_memory ,"/" );
#endif
		this->_i_dir_part_length = 0;
	}

	/* アプリ立ち上げ時の初期設定で使う */
	void setup_path( const char *path );

protected:
	const char *ccp_image_file_ext( void ) const
	{ return this->_ccp_image_file_ext; }

	/* ファイル名が数字と拡張子をもったものかどうか調べる */
	const char *ccp_num4_and_ext(const char *ccp_file);

	/* ディレクトリとファイル名と拡張子からファイルパスを作る */
	int i_path_dir_file( const char *ccp_dir, const char *ccp_file,
		const char *ccp_ext = NULL );

	/* 現在ディレクトリ(ccp_dir)から下(ccp_down_dir)へ */
	int i_path_dir_down( const char *ccp_dir, const char *ccp_down_dir );

	/* 現在ディレクトリ(ccp_dir)から上へ */
	int i_path_dir_up( const char *ccp_dir );

	void v_path_dir_part_zero( void ) {
		this->_i_dir_part_length = 0;
	}
	void v_path_dir_part_cutting( void ) {
		this->_ca_path[this->_i_dir_part_length] = '\0';
	}

	void v_path_memory( void );
	void v_path_memory( const char *ccp );

	char *cp_memory( void );

	/* できたパスを見る、あるいは別途加工する */
	char *cp_path( void ) { return this->_ca_path; }

	/* ---------------------------------------------- */

	/* 頭にディレクトリをドーンと置く */
	int i_path_cpy_dir( const char *ccp_src,
		char *cp_dest = NULL, int i_dest_len = PTBL_PATH_MAX );

	/* パスの部品を後ろにつなげていく */
	int i_path_cat_part( const char *ccp_src,
		char *cp_dest = NULL, int i_dest_len = PTBL_PATH_MAX );

	/* ファイル名からその頭名を、後ろにつなげる */
	int i_path_cat_head( const char *ccp_src,
		int i_all_cat_sw=OFF,
		char *cp_dest=NULL, int i_dest_len=PTBL_PATH_MAX );

	/* 番号を4桁のゼロ詰めにして、後ろにつなげる */
	int i_path_cat_pad4( int i_num,
		char *cp_dest = NULL, int i_dest_len = PTBL_PATH_MAX );

	/* 拡張子はブラウザーからの選択指定なのでここでは指定しない */

private:
	const char *_ccp_image_file_ext;

	/* 上記各methodでここにpath設定 */
	char _ca_path[PTBL_PATH_MAX];

	int _i_dir_part_length;

	/* GUIのCancel復帰用 */
	char _ca_memory[PTBL_PATH_MAX];
};

#endif /* !__gtsfbro01path_base_h__ */
