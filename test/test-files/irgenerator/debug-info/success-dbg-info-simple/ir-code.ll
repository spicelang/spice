; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { i64, %struct.String, i32 }
%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1, !dbg !0
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Long: %d\0A\00", align 1, !dbg !5
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 1, !dbg !9
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1, !dbg !12

; Function Attrs: norecurse
define void @_ZN10TestStruct4dtorEv(ptr noundef nonnull align 8 dereferenceable(40) %0) #0 !dbg !23 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8, !dbg !43
    #dbg_declare(ptr %this, !44, !DIExpression(), !43)
  %2 = load ptr, ptr %this, align 8, !dbg !43
  %3 = getelementptr inbounds nuw %struct.TestStruct, ptr %2, i32 0, i32 1, !dbg !43
  call void @_ZN6String4dtorEv(ptr %3), !dbg !43
  ret void, !dbg !43
}

declare void @_ZN6String4dtorEv(ptr)

define private %struct.TestStruct @_Z3fctRi(ptr %0) !dbg !46 {
  %result = alloca %struct.TestStruct, align 8
  %ref = alloca ptr, align 8
  %2 = alloca %struct.String, align 8
  %ts = alloca %struct.TestStruct, align 8
    #dbg_declare(ptr %result, !50, !DIExpression(), !51)
  store ptr %0, ptr %ref, align 8, !dbg !52
    #dbg_declare(ptr %ref, !53, !DIExpression(), !52)
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.0), !dbg !54
  store i64 6, ptr %ts, align 8, !dbg !55
  %3 = load %struct.String, ptr %2, align 8, !dbg !55
  %4 = getelementptr inbounds nuw %struct.TestStruct, ptr %ts, i32 0, i32 1, !dbg !55
  store %struct.String %3, ptr %4, align 8, !dbg !55
  %5 = load ptr, ptr %ref, align 8, !dbg !55
  %6 = load i32, ptr %5, align 4, !dbg !55
  %7 = getelementptr inbounds nuw %struct.TestStruct, ptr %ts, i32 0, i32 2, !dbg !55
  store i32 %6, ptr %7, align 4, !dbg !55
    #dbg_declare(ptr %ts, !56, !DIExpression(), !57)
  %8 = load %struct.TestStruct, ptr %ts, align 8, !dbg !58
  ret %struct.TestStruct %8, !dbg !59
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 !dbg !60 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %res = alloca %struct.TestStruct, align 8
    #dbg_declare(ptr %result, !63, !DIExpression(), !64)
  store i32 0, ptr %result, align 4, !dbg !64
  store i32 987654, ptr %test, align 4, !dbg !65
    #dbg_declare(ptr %test, !66, !DIExpression(), !67)
  %1 = call %struct.TestStruct @_Z3fctRi(ptr %test), !dbg !68
  store %struct.TestStruct %1, ptr %res, align 8, !dbg !68
    #dbg_declare(ptr %res, !69, !DIExpression(), !70)
  %lng.addr = getelementptr inbounds %struct.TestStruct, ptr %res, i64 0, i32 0, !dbg !71
  %2 = load i64, ptr %lng.addr, align 8, !dbg !71
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %2), !dbg !71
  %4 = getelementptr inbounds nuw %struct.TestStruct, ptr %res, i32 0, i32 1, !dbg !72
  %5 = call ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %4), !dbg !72
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5), !dbg !72
  %i.addr = getelementptr inbounds %struct.TestStruct, ptr %res, i64 0, i32 2, !dbg !73
  %7 = load i32, ptr %i.addr, align 4, !dbg !73
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %7), !dbg !73
  call void @_ZN10TestStruct4dtorEv(ptr %res), !dbg !74
  %9 = load i32, ptr %result, align 4, !dbg !74
  ret i32 %9, !dbg !74
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #2

declare ptr @_ZN6String6getRawEv(ptr)

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!16, !17, !18, !19, !20, !21}
!llvm.ident = !{!22}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "anon.string.0", linkageName: "anon.string.0", scope: !2, file: !7, line: 8, type: !15, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/debug-info/success-dbg-info-simple/source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-simple")
!4 = !{!0, !5, !9, !12}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !7, line: 15, type: !8, isLocal: true, isDefinition: true)
!7 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-simple")
!8 = !DIStringType(name: "printf.str.0", size: 80)
!9 = !DIGlobalVariableExpression(var: !10, expr: !DIExpression())
!10 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !7, line: 16, type: !11, isLocal: true, isDefinition: true)
!11 = !DIStringType(name: "printf.str.1", size: 96)
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !7, line: 17, type: !14, isLocal: true, isDefinition: true)
!14 = !DIStringType(name: "printf.str.2", size: 72)
!15 = !DIStringType(name: "anon.string.0", size: 96)
!16 = !{i32 8, !"PIC Level", i32 2}
!17 = !{i32 7, !"PIE Level", i32 2}
!18 = !{i32 7, !"uwtable", i32 2}
!19 = !{i32 7, !"frame-pointer", i32 2}
!20 = !{i32 7, !"Dwarf Version", i32 5}
!21 = !{i32 2, !"Debug Info Version", i32 3}
!22 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!23 = distinct !DISubprogram(name: "dtor", linkageName: "_ZN10TestStruct4dtorEv", scope: !7, file: !7, line: 1, type: !24, scopeLine: 1, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !42)
!24 = !DISubroutineType(types: !25)
!25 = !{!26, !27}
!26 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!27 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !28, identifier: "struct.TestStruct")
!28 = !{!29, !31, !40}
!29 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !27, file: !7, line: 2, baseType: !30, size: 64)
!30 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !27, file: !7, line: 3, baseType: !32, size: 192, align: 8, offset: 64)
!32 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !7, file: !7, line: 21, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !33, identifier: "struct.String")
!33 = !{!34, !37, !39}
!34 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !32, file: !7, line: 22, baseType: !35, size: 64)
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !36, size: 64)
!36 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!37 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !32, file: !7, line: 23, baseType: !38, size: 64, offset: 64)
!38 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!39 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !32, file: !7, line: 24, baseType: !38, size: 64, offset: 128)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !27, file: !7, line: 4, baseType: !41, size: 32, offset: 256)
!41 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!42 = !{}
!43 = !DILocation(line: 1, column: 1, scope: !23)
!44 = !DILocalVariable(name: "this", arg: 1, scope: !23, file: !7, line: 1, type: !45)
!45 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !27, size: 64)
!46 = distinct !DISubprogram(name: "fct", linkageName: "_Z3fctRi", scope: !7, file: !7, line: 7, type: !47, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !42)
!47 = !DISubroutineType(types: !48)
!48 = !{!27, !49}
!49 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !41, size: 64)
!50 = !DILocalVariable(name: "result", scope: !46, file: !7, line: 7, type: !27)
!51 = !DILocation(line: 7, column: 1, scope: !46)
!52 = !DILocation(line: 7, column: 19, scope: !46)
!53 = !DILocalVariable(name: "ref", arg: 1, scope: !46, file: !7, line: 7, type: !49)
!54 = !DILocation(line: 8, column: 44, scope: !46)
!55 = !DILocation(line: 8, column: 60, scope: !46)
!56 = !DILocalVariable(name: "ts", scope: !46, file: !7, line: 8, type: !27)
!57 = !DILocation(line: 8, column: 5, scope: !46)
!58 = !DILocation(line: 9, column: 12, scope: !46)
!59 = !DILocation(line: 10, column: 1, scope: !46)
!60 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !7, file: !7, line: 12, type: !61, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !42)
!61 = !DISubroutineType(types: !62)
!62 = !{!41}
!63 = !DILocalVariable(name: "result", scope: !60, file: !7, line: 12, type: !41)
!64 = !DILocation(line: 12, column: 1, scope: !60)
!65 = !DILocation(line: 13, column: 16, scope: !60)
!66 = !DILocalVariable(name: "test", scope: !60, file: !7, line: 13, type: !41)
!67 = !DILocation(line: 13, column: 5, scope: !60)
!68 = !DILocation(line: 14, column: 32, scope: !60)
!69 = !DILocalVariable(name: "res", scope: !60, file: !7, line: 14, type: !27)
!70 = !DILocation(line: 14, column: 5, scope: !60)
!71 = !DILocation(line: 15, column: 26, scope: !60)
!72 = !DILocation(line: 16, column: 28, scope: !60)
!73 = !DILocation(line: 17, column: 25, scope: !60)
!74 = !DILocation(line: 18, column: 1, scope: !60)
