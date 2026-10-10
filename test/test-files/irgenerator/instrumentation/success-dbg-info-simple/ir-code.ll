; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { i64, %struct.String, i32 }
%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 4, !dbg !0
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Long: %d\0A\00", align 4, !dbg !5
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 4, !dbg !10
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 4, !dbg !13

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN10TestStruct4dtorEv(ptr noundef nonnull align 8 dereferenceable(40) %0) #0 !dbg !23 {
  %this = alloca ptr, align 8
    #dbg_declare(ptr %this, !42, !DIExpression(), !44)
  store ptr %0, ptr %this, align 8, !dbg !44
  %2 = load ptr, ptr %this, align 8, !dbg !44
  %3 = getelementptr inbounds nuw %struct.TestStruct, ptr %2, i32 0, i32 1, !dbg !44
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %3), !dbg !44
  ret void, !dbg !44
}

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z3fctRi(ptr dead_on_unwind noalias writable sret(%struct.TestStruct) align 8 %0, ptr noundef %1) #1 !dbg !45 {
  %ref = alloca ptr, align 8
  %3 = alloca %struct.String, align 8
  %ts = alloca %struct.TestStruct, align 8
    #dbg_declare(ptr %ref, !50, !DIExpression(), !52)
  store ptr %1, ptr %ref, align 8, !dbg !52
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %3, ptr noundef @anon.string.0), !dbg !53
  store i64 6, ptr %ts, align 8, !dbg !54
  %4 = load %struct.String, ptr %3, align 8, !dbg !54
  %5 = getelementptr inbounds nuw %struct.TestStruct, ptr %ts, i32 0, i32 1, !dbg !54
  store %struct.String %4, ptr %5, align 8, !dbg !54
  %6 = load ptr, ptr %ref, align 8, !dbg !54
  %7 = load i32, ptr %6, align 4, !dbg !54
  %8 = getelementptr inbounds nuw %struct.TestStruct, ptr %ts, i32 0, i32 2, !dbg !54
  store i32 %7, ptr %8, align 4, !dbg !54
    #dbg_declare(ptr %ts, !51, !DIExpression(), !54)
  %9 = load %struct.TestStruct, ptr %ts, align 8, !dbg !55
  store %struct.TestStruct %9, ptr %0, align 8, !dbg !56
  ret void, !dbg !56
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #2 !dbg !57 {
  %test = alloca i32, align 4
  %res = alloca %struct.TestStruct, align 8
    #dbg_declare(ptr %test, !61, !DIExpression(), !63)
  store i32 987654, ptr %test, align 4, !dbg !63
  call void @_Z3fctRi(ptr dead_on_unwind writable sret(%struct.TestStruct) align 8 %res, ptr noundef %test), !dbg !64
    #dbg_declare(ptr %res, !62, !DIExpression(), !64)
  %lng.addr = getelementptr inbounds %struct.TestStruct, ptr %res, i64 0, i32 0, !dbg !65
  %1 = load i64, ptr %lng.addr, align 8, !dbg !65
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef %1), !dbg !65
  %3 = getelementptr inbounds nuw %struct.TestStruct, ptr %res, i32 0, i32 1, !dbg !66
  %4 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %3), !dbg !66
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %4), !dbg !66
  %i.addr = getelementptr inbounds %struct.TestStruct, ptr %res, i64 0, i32 2, !dbg !67
  %6 = load i32, ptr %i.addr, align 4, !dbg !67
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %6), !dbg !67
  call void @_ZN10TestStruct4dtorEv(ptr noundef nonnull align 8 dereferenceable(40) %res), !dbg !68
  ret i32 0, !dbg !68
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

declare ptr @_ZN6String6getRawEv(ptr)

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!16, !17, !18, !19, !20, !21}
!llvm.ident = !{!22}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "anon.string.0", linkageName: "anon.string.0", scope: !2, file: !3, line: 10, type: !12, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false)
!3 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-simple")
!4 = !{!0, !5, !10, !13}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !3, line: 17, type: !7, isLocal: true, isDefinition: true)
!7 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 80, elements: !9)
!8 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!9 = !{}
!10 = !DIGlobalVariableExpression(var: !11, expr: !DIExpression())
!11 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !3, line: 18, type: !12, isLocal: true, isDefinition: true)
!12 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 96, elements: !9)
!13 = !DIGlobalVariableExpression(var: !14, expr: !DIExpression())
!14 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !3, line: 19, type: !15, isLocal: true, isDefinition: true)
!15 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 72, elements: !9)
!16 = !{i32 8, !"PIC Level", i32 2}
!17 = !{i32 7, !"PIE Level", i32 2}
!18 = !{i32 7, !"uwtable", i32 2}
!19 = !{i32 7, !"frame-pointer", i32 0}
!20 = !{i32 7, !"Dwarf Version", i32 5}
!21 = !{i32 2, !"Debug Info Version", i32 3}
!22 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!23 = distinct !DISubprogram(name: "dtor", linkageName: "_ZN10TestStruct4dtorEv", scope: !3, file: !3, line: 3, type: !24, scopeLine: 3, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !41)
!24 = !DISubroutineType(types: !25)
!25 = !{!26, !27}
!26 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!27 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !3, file: !3, line: 3, size: 320, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !28, identifier: "struct.TestStruct")
!28 = !{!29, !31, !39}
!29 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !27, file: !3, line: 4, baseType: !30, size: 64)
!30 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !27, file: !3, line: 5, baseType: !32, size: 192, align: 64, offset: 64)
!32 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !3, file: !3, line: 29, size: 192, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !33, identifier: "struct.String")
!33 = !{!34, !36, !38}
!34 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !32, file: !3, line: 30, baseType: !35, size: 64)
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !8, size: 64, dwarfAddressSpace: 0)
!36 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !32, file: !3, line: 31, baseType: !37, size: 64, offset: 64)
!37 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!38 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !32, file: !3, line: 32, baseType: !37, size: 64, offset: 128)
!39 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !27, file: !3, line: 6, baseType: !40, size: 32, offset: 256)
!40 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!41 = !{!42}
!42 = !DILocalVariable(name: "this", arg: 1, scope: !23, file: !3, line: 3, type: !43)
!43 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !27, size: 64, dwarfAddressSpace: 0)
!44 = !DILocation(line: 3, column: 1, scope: !23)
!45 = distinct !DISubprogram(name: "fct", linkageName: "_Z3fctRi", scope: !3, file: !3, line: 9, type: !46, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !49)
!46 = !DISubroutineType(types: !47)
!47 = !{!27, !48}
!48 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !40)
!49 = !{!50, !51}
!50 = !DILocalVariable(name: "ref", arg: 1, scope: !45, file: !3, line: 9, type: !48)
!51 = !DILocalVariable(name: "ts", scope: !45, file: !3, line: 10, type: !27)
!52 = !DILocation(line: 9, column: 19, scope: !45)
!53 = !DILocation(line: 10, column: 44, scope: !45)
!54 = !DILocation(line: 10, column: 60, scope: !45)
!55 = !DILocation(line: 11, column: 12, scope: !45)
!56 = !DILocation(line: 12, column: 1, scope: !45)
!57 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !3, file: !3, line: 14, type: !58, scopeLine: 14, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !60)
!58 = !DISubroutineType(types: !59)
!59 = !{!40}
!60 = !{!61, !62}
!61 = !DILocalVariable(name: "test", scope: !57, file: !3, line: 15, type: !40)
!62 = !DILocalVariable(name: "res", scope: !57, file: !3, line: 16, type: !27)
!63 = !DILocation(line: 15, column: 16, scope: !57)
!64 = !DILocation(line: 16, column: 32, scope: !57)
!65 = !DILocation(line: 17, column: 26, scope: !57)
!66 = !DILocation(line: 18, column: 28, scope: !57)
!67 = !DILocation(line: 19, column: 25, scope: !57)
!68 = !DILocation(line: 20, column: 1, scope: !57)
