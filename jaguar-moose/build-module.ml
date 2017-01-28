<project name="build-jaguar-moose" default="module-build">

	<property name="module-name" value="jaguar-moose"/>

	<cc-config id="lib-set-jaguar-moose">
		<cc-config refid="module.caterpillar.config"/>
		<cc-config refid="module.shoveler.config"/>
		<cc-config refid="module.worm.config"/>
		<cc-config refid="module.leafhopper.config"/>
		<cc-config refid="module.astarte.config"/>
		<cc-config refid="module.viper.config"/>
		<cc-config refid="module.moose.config"/>
		<cc-config refid="module.moose.explorer.config"/>
		<cc-config refid="module.elk.config"/>
		<cc-config refid="module.jaguar.bytecode.config"/>
		<cc-config refid="module.jaguar.decompiler.config"/>
		<cc-config refid="module.gtk3.config"/>
	</cc-config>
	

	<cc-config id="lib-set-ut-jaguar-moose">
		<cc-config refid="module.ut.caterpillar.config"/>
		<cc-config refid="module.ut.shoveler.config"/>
		<cc-config refid="module.ut.worm.config"/>
		<cc-config refid="module.ut.leafhopper.config"/>
		<cc-config refid="module.ut.astarte.config"/>
		<cc-config refid="module.ut.viper.config"/>
		<cc-config refid="module.ut.moose.config"/>
		<cc-config refid="module.ut.moose.explorer.config"/>
		<cc-config refid="module.ut.elk.config"/>
		<cc-config refid="module.ut.jaguar.bytecode.config"/>
		<cc-config refid="module.ut.jaguar.decompiler.config"/>
		<cc-config refid="module.gtk3.config"/>
	</cc-config>
		
	<import file="${natpad.build}/module.inc" optional="false"/>


	<!--target name="module-ut-build" depends="module-ut-compile, module-ut-archive">
		<clink srcdir="build/ut/test/" output="build/${module-name}-ut">
			<cc-config refid="module.ut.${module-name}.config"/>
			<cc-config refid="lib-set-ut-${module-name}"/>
			<cc-config refid="module.cc.dbg.config"/>
			<cc-strip-argument argument="-lrt"/>
		</clink>
	</target-->



</project>
