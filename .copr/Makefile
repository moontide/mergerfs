get-version-number:
	dnf install -y git
	tools/update-version

create-tar-file: get-version-number
	$(eval VERSION := $(shell cat VERSION))
	$(eval VERSION := $(subst -,_,$(VERSION)))
	$(eval FILENAME := mergerfs-$(VERSION))
	$(eval TMPDIR := $(shell mktemp --tmpdir -d $(FILENAME).XXXXXXXX))
	mkdir $(TMPDIR)/$(FILENAME)
	cp -ar . $(TMPDIR)/$(FILENAME)
	tar --exclude=.git --exclude=.copr -cz -C $(TMPDIR) -f $(FILENAME).tar.gz $(FILENAME)
	rm -rf $(TMPDIR)

srpm: create-tar-file
	$(eval TMP_RPMBUILD_DIR := $(shell mktemp --tmpdir -d rpmbuild-dir-XXXX))
	mkdir -p $(TMP_RPMBUILD_DIR)/{BUILD,SRPMS,SOURCES,SPECS}
	mv $(FILENAME).tar.gz $(TMP_RPMBUILD_DIR)/SOURCES

	# 生成 libfuse/Makefile 的补丁文件。
	# 注意，diff 命令会返回非 0 的结果，会导致 make 认为 diff 命令失败而结束执行，需要用 || exit 0 来确保该行的结果为 0。参考： https://stackoverflow.com/questions/9657205/makefile-failing-when-using-diff-on-different-files
	$(eval patch_file_name_of_libfuse_Makefile := 0-disable-chown-in-libfuse-Makefile.patch)
	sed 's/^[[:blank:]]*chown/#    chown/' libfuse/Makefile > $(TMP_RPMBUILD_DIR)/libfuse-Makefile-modified
	diff -u libfuse/Makefile $(TMP_RPMBUILD_DIR)/libfuse-Makefile-modified > $(TMP_RPMBUILD_DIR)/SOURCES/$(patch_file_name_of_libfuse_Makefile) || exit 0

	# 修改 mergerfs.spec，使其能对 libfuse/Makefile 打补丁
	$(eval patched_spec_file_name := $(TMP_RPMBUILD_DIR)/SPECS/mergerfs-for-fedora-copr.spec)
	sed 's/__VERSION__/$(VERSION)/g' mergerfs.spec > $(patched_spec_file_name)
	sed -i '/^Source:/a Patch0:	$(patch_file_name_of_libfuse_Makefile)' $(patched_spec_file_name)
	sed -i '/^%setup -q/a %patch0 -p0' $(patched_spec_file_name)

	# 打包 srpm
	rpmbuild --target noarch -bs $(patched_spec_file_name)  --define "_topdir $(TMP_RPMBUILD_DIR)"
	cp -p $(TMP_RPMBUILD_DIR)/SRPMS/* $(outdir)
