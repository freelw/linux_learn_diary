#!/bin/bash
#set -x

print_usage()
{
	echo "Usage: one_click_crash [vmcorefile] [-d|-g|-m|-p|-h|-s|-b]"
	echo "-d debug mode, check os-release"
	echo "-g generate dmesg then quit"
	echo "-m analyze memory"
	echo "-p analyze ps"
	echo "-h analyze hung task (not implemented yet)"
	echo "-s analyze softlock (not implemented yet)"
	echo "-b analyze BUG/others (not implemented yet)"
}

if [ $# -lt 1 ]
then
	print_usage
	exit 0
fi

dir=`pwd`
vmcorefile=
vmlinuxfile=
arch=
os=

#empty means debug option off, otherwise on
debug=

memory=
ps=
hungtask=
softlock=
bug=
other=

function parse_args()
{
	vmcorefile=$1
	shift
	arg=

	if [ -f "crash_ins.txt" ]
	then
		rm "crash_ins.txt"
	fi

	echo """echo ' '""" >> "crash_ins.txt"
	echo "sys" >> "crash_ins.txt"

	while [ x$1 != x ]
	do
		arg=$1
		shift
		arg=`echo $arg | sed 's/-//g'`

		case $arg in
			d)	debug=1
			;;
			g)
				gen_dmesg
			;;
			m)	memory=1
				gen_mem
			;;
			p)	ps=1
				gen_ps
			;;
			h)	hungtask=1
				gen_hung
			;;
			s)	softlock=1
				gen_soft
			;;
			b)	bug=1
				gen_bug
			;;
			*)	other=1
				gen_other
			;;
		esac
	done

	osrelease=`crash --osrelease $vmcorefile`
	error "check vmcorefile osrelease"
}

function gen_mem()
{
	echo """echo ' '""" >> "crash_ins.txt"
	echo """echo 'Detail memory distribution:'""" >> "crash_ins.txt"
	echo "kmem -V" >> "crash_ins.txt"
	echo """echo ' '""" >> "crash_ins.txt"

	echo """echo 'Memory usage:'""" >> "crash_ins.txt"
	echo "kmem -i" >> "crash_ins.txt"
	echo """echo ' '""" >> "crash_ins.txt"

	echo """echo 'Processes sorted by memory usage（categorized by name, shared memory included):'""" >> "crash_ins.txt"
	echo "ps -G | sed 's/^>//' | awk '{ m[\$9]+=\$8/1024 } END { for (item in m) { printf \"%20s %10s MiB\n\", item, m[item] } }' | sort -k 2 -r -n | head" >> "crash_ins.txt"
	echo """echo ' '""" >> "crash_ins.txt"

	# statistic with PID: useless most of time
	#echo """echo 'Processes sorted by memory usage（PID added, name debut):'""" >> "crash_ins.txt"
	#echo "ps -G | sed 's/^>//' | awk '{ m[\$9]+=\$8/1024; p[\$9]=\$1 } END { for (item in m) { printf \"%s %20s %10s MiB\n\", p[item], item, m[item] } }' | sort -k 3 -r -n | head" >> "crash_ins.txt"

}

function gen_ps()
{
	echo """echo ' '""" >> "crash_ins.txt"
	echo """echo 'Detail ps distribution:'""" >> "crash_ins.txt"
	echo " ps | sed 's/^>//' | awk '{ m[\$9]+=1 } END { for (item in m) { printf \"%s %s \n\", item, m[item] } }' | sort -k 2 -r -n | head" >> "crash_ins.txt"
}

#TODO
function gen_hung()
{
	echo "log | grep block"  >> "crash_ins.txt"
	echo "ps -l | grep UN | tail"  >> "crash_ins.txt"
}

function gen_soft()
{
	echo "log | grep stuck"  >> "crash_ins.txt"
	echo "bt"  >> "crash_ins.txt"
	echo "runq" >> "crash_ins.txt"
}

function gen_bug()
{
	echo "bt" >> "crash_ins.txt"
}

function gen_other()
{
	echo "bt" >> "crash_ins.txt"
}

function gen_dmesg()
{
	echo "dmesg > dmesg_file" >> "crash_ins.txt"
	echo "q" >> "crash_ins.txt"
}
#

error()
{
	if [ $? != 0 ]
	then
		echo "Error after $1, please check!"
		exit 1
	fi
}

function install_debuginfo()
{
	rpm -ivh --force $1
    if [ $? != 0 ]
    then
        rpm2cpio $1 | cpio -idmv
        error "extract debuginfo rpm"
        vmlinuxfile=$dir"/boot/vmlinux-$2"
    else
        vmlinuxfile="/boot/vmlinux-$2"
	fi
}

function exec_crash()
{
	ins=
	if [ -f "crash_ins.txt" ]
	then
		ins="-i crash_ins.txt"
	fi

	if [ x$debug == x ]
	then
		crash -s $vmlinuxfile $vmcorefile $ins
	else
		echo "Debug mode: kernel-release is $osrelease, check passed"
	fi
}

exec_centos()
{
	mirror="http://debuginfo.centos.org"
	ret=`curl -I -m 5 -s -o /dev/null -w %{http_code} $mirror`

	if [ "x$ret" != "x200" ]
	then
		mirror="http://mirrors.tencent.com/centos-debuginfo"
	fi

	suffix=
	if [ "x$1" == "x8" ]
	then
		suffix="Packages/"
	fi

	if [ -f "/usr/lib/debug/lib/modules/$osrelease/vmlinux" ]
	then
		vmlinuxfile="/usr/lib/debug/lib/modules/$osrelease/vmlinux"
	elif [ -f $dir"/usr/lib/debug/lib/modules/$osrelease/vmlinux" ]
	then
		vmlinuxfile=$dir"/usr/lib/debug/lib/modules/$osrelease/vmlinux"
	else
		rm -f kernel-debuginfo-*

		wget "$mirror/$1/$2/"$suffix"kernel-debuginfo-common-$2-$osrelease.rpm"
		error "wget debuginfo-common"

		wget "$mirror/$1/$2/"$suffix"kernel-debuginfo-$osrelease.rpm"
		error "wget debuginfo"

		rpm -ivh --force "kernel-debuginfo-common-$2-$osrelease.rpm"
		rpm -ivh --force "kernel-debuginfo-$osrelease.rpm"
		if [ $? != 0 ]
		then
			rpm2cpio "kernel-debuginfo-$osrelease.rpm" | cpio -idmv
			error "extract debuginfo rpm"
			vmlinuxfile=$dir"/usr/lib/debug/lib/modules/$osrelease/vmlinux"
		else
			vmlinuxfile="/usr/lib/debug/lib/modules/$osrelease/vmlinux"
		fi
	fi

	exec_crash
}

exec_tlinux()
{
	if [ -f "/boot/vmlinux-$osrelease" ]
	then
		vmlinuxfile="/boot/vmlinux-$osrelease"
	elif [ -f $dir"/boot/vmlinux-$osrelease" ]
	then
		vmlinuxfile=$dir"/boot/vmlinux-$osrelease"
	else
		rm -f kernel-*
		if [ `echo $1 | grep "tvmguest"` ]
		then
			osrelease_download="https://mirrors.tencent.com/tlinux/1.2/tlinux/debuginfo/x86_64/RPMS/kernel-tlinux-debuginfo-2.6.32.43-1.0.27.tl1.x86_64.rpm"
			wget $osrelease_download
			error "wget debuginfo"

			install_debuginfo "kernel-tlinux-debuginfo-2.6.32.43-1.0.27.tl1.x86_64.rpm" $osrelease
		elif [ `echo $1 | grep "kvm_guest"` ]
		then
			osrelease_download=`echo "$osrelease" | sed  "s/-tlinux2_kvm_guest-/./"`".tl2."$2".rpm"
			wget "http://mirrors.tencent.com/tlinux/2.2/tlinux-kvm-guest/debuginfo/$2/RPMS/kernel-tlinux2_kvm_guest-debuginfo-$osrelease_download"
			error "wget debuginfo"

			install_debuginfo "kernel-tlinux2_kvm_guest-debuginfo-$osrelease_download" $osrelease
		elif [ `echo $1 | grep "3.10"` ]
		then
			middle_version=`echo "$osrelease" | awk -F "-" '{printf $3}'`
			osrelease_download=`echo "$osrelease" | sed  "s/-$middle_version-/./"`".tl2."$2".rpm"
			wget "http://mirrors.tencent.com/tlinux/2.2/tlinux/debuginfo/$2/RPMS/kernel-$middle_version-debuginfo-$osrelease_download"

			# if failed try another source
			if [ $? != 0 ]
			then
				wget "http://mirrors.tencent.com/os/tlinux/Tlinux-Kernel-RPMs/$osrelease/kernel-$middle_version-debuginfo-$osrelease_download"
			fi

			error "wget debuginfo"

			install_debuginfo "kernel-$middle_version-debuginfo-$osrelease_download" $osrelease
		elif [ `echo $1 | grep "4.14"` ]
		then
			middle_version=`echo "$osrelease" | awk -F "-" '{printf $3}'`
			if [ x$middle_version == x"tlinux3" ]
			then
				osrelease_download=`echo "$osrelease" | sed  "s/-$middle_version-/./"`".tl2."$2".rpm"
				wget "http://mirrors.tencent.com/tlinux/2.2/tlinux-tkernel3/debuginfo/$2/RPMS/kernel-$middle_version-debuginfo-$osrelease_download"
				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/tlinux/2.6/tlinux/debuginfo/$2/RPMS/kernel-$middle_version-debuginfo-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/os/tlinux/Tlinux-Kernel-RPMs/$osrelease/kernel-$middle_version-debuginfo-$osrelease_download"
				fi
				middle_version="$middle_version"-
			else
				osrelease_download=`echo "$osrelease" | sed "s/-/./2"`".tl2."$2".rpm"
				wget "http://mirrors.tencent.com/tlinux/2.4/tlinux/debuginfo/$2/RPMS/kernel-debuginfo-$osrelease_download"

				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/os/tlinux_public/kernel/$2/kernel-debuginfo-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/os/tlinux_public/kernel/$2/$osrelease/kernel-debuginfo-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/os/tlinux/Tlinux-Kernel-RPMs/$osrelease/kernel-debuginfo-$osrelease_download"
				fi
				middle_version=
			fi
			error "wget debuginfo"
			install_debuginfo "kernel-"$middle_version"debuginfo-$osrelease_download" $osrelease
		elif [ `echo $1 | grep "5.4"` ]
		then
			middle_version=`echo "$osrelease" | awk -F "-" '{printf $3}'`
			if [ x$middle_version == x"tlinux4" ]
			then
				osrelease_download=`echo "$osrelease" | sed  "s/-$middle_version-/./"`".tl2."$2".rpm"
				wget "http://mirrors.tencent.com/tlinux/2.6/tlinux-tkernel4/debuginfo/$2/RPMS/kernel-$middle_version-debuginfo-common-$osrelease_download"
				if [ $? != 0 ]
				then
					osrelease_download=`echo "$osrelease" | sed  "s/-$middle_version-/./"`".tl3."$2".rpm"
					wget "http://mirrors.tencent.com/tlinux/3/TencentOS/debuginfo/$2/RPMS/kernel-$middle_version-debuginfo-common-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					osrelease_download=`echo "$osrelease" | sed  "s/-$middle_version-/./"`".tl3."$2".rpm"
					wget "http://mirrors.tencent.com/tlinux/3.2/TencentOS/debuginfo/$2/RPMS/kernel-$middle_version-debuginfo-common-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/os/tlinux/Tlinux-Kernel-RPMs/$osrelease/kernel-$middle_version-debuginfo-common-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					osrelease_download=`echo "$osrelease" | sed  "s/-$middle_version-/./"`".tl3."$2".rpm"
					wget "http://mirrors.tencent.com/os/tlinux/Tlinux-Kernel-RPMs/$osrelease/kernel-$middle_version-debuginfo-common-$osrelease_download"
				fi
				middle_version="$middle_version"-
			else
				osrelease_download=`echo "$osrelease" | sed  "s/-/./2"`".tl3."$2".rpm"
				wget "http://mirrors.tencent.com/tlinux/3.1/TencentOS/debuginfo/$2/RPMS/kernel-debuginfo-common-$osrelease_download"
				if [ $? != 0 ]
				then
					osrelease_download=`echo "$osrelease" | sed  "s/-/./2"`".tl2."$2".rpm"
					wget "http://mirrors.tencent.com/tlinux/2.4/tlinux-tkernel4/debuginfo/$2/RPMS/kernel-debuginfo-common-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					wget "http://mirrors.tencent.com/os/tlinux_public/kernel/$2/$2-$osrelease/kernel-debuginfo-common-$osrelease_download"
				fi
				if [ $? != 0 ]
				then
					osrelease_download=`echo "$osrelease" | sed  "s/-/./2"`".tl2."$2".rpm"
					wget "http://mirrors.tencent.com/os/tlinux_public/kernel/$2/$2-$osrelease/kernel-debuginfo-common-$osrelease_download"
				fi
				middle_version=
			fi
			error "wget debuginfo"
			install_debuginfo "kernel-"$middle_version"debuginfo-common-$osrelease_download" $osrelease
		fi
	fi

	exec_crash
}

check_os()
{
	if [ `echo $1 | grep "x86_64"` ]
	then
		arch="x86_64"
	elif [ `echo $1 | grep "aarch64"` ]
	then
		arch="aarch64"
	elif [ `echo $1 | grep "armhfp"` ]
	then
		arch="armhfp"
	elif [ `echo $1 | grep "i386"` ]
	then
		arch="i386"
	elif [ `echo $1 | grep "ppc64"` ]
	then
		arch="ppc64"
	elif [ `echo $1 | grep "ppc64le"` ]
	then
		arch="ppc64le"
	else
		# TODO: deal with other arch if necessary
		echo "Arch unrecognized choose default as x86_64!"
		arch="x86_64"
	fi

	if [ `echo $1 | grep "el7"` ]
	then
		os="el7"
		exec_centos "7" $arch
	elif [ `echo $1 | grep "el6"` ]
	then
		os="el6"
		exec_centos "6" $arch
	elif [ `echo $1 | grep "el8"` ]
	then
		os="el8"
		exec_centos "8" $arch
	elif [ `echo $1 | grep "tl3"` ]
	then
		os="tl3"
		exec_tlinux "tl3" $arch
	else
		os="tlinux"
		exec_tlinux $1 $arch
	fi
}

parse_args $*
check_os $osrelease