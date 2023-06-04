; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"__TestStruct__long___rt_string::String_int" = type { i64, %__String__charptr_long_long, i32 }
%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1, !dbg !0
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Long: %d\0A\00", align 1, !dbg !5
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 1, !dbg !9
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1, !dbg !12

define private %"__TestStruct__long___rt_string::String_int" @_f__void__TestStruct__fct__intref(ptr %0) !dbg !24 {
  %result = alloca %"__TestStruct__long___rt_string::String_int", align 8
  %ref = alloca ptr, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %ts = alloca %"__TestStruct__long___rt_string::String_int", align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !44, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.declare(metadata ptr %ref, metadata !56, metadata !DIExpression()), !dbg !55
  store ptr %0, ptr %ref, align 8, !dbg !55
  call void @_mp__String__void__ctor__string(ptr %2, ptr @anon.string.0), !dbg !57
  %3 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %ts, i32 0, i32 0, !dbg !58
  store i64 6, ptr %3, align 8, !dbg !58
  %4 = load %__String__charptr_long_long, ptr %2, align 8, !dbg !58
  %5 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %ts, i32 0, i32 1, !dbg !58
  store %__String__charptr_long_long %4, ptr %5, align 8, !dbg !58
  %6 = load ptr, ptr %ref, align 8, !dbg !58
  %7 = load i32, ptr %6, align 4, !dbg !58
  %8 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %ts, i32 0, i32 2, !dbg !58
  call void @llvm.dbg.declare(metadata ptr %ts, metadata !59, metadata !DIExpression()), !dbg !58
  store i32 %7, ptr %8, align 4, !dbg !58
  %9 = load %"__TestStruct__long___rt_string::String_int", ptr %ts, align 8, !dbg !70
  ret %"__TestStruct__long___rt_string::String_int" %9, !dbg !70
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #0

declare void @_mp__String__void__ctor__string(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 !dbg !71 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %res = alloca %"__TestStruct__long___rt_string::String_int", align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !74, metadata !DIExpression()), !dbg !75
  store i32 0, ptr %result, align 4, !dbg !75
  call void @llvm.dbg.declare(metadata ptr %test, metadata !76, metadata !DIExpression()), !dbg !77
  store i32 987654, ptr %test, align 4, !dbg !77
  %1 = call %"__TestStruct__long___rt_string::String_int" @_f__void__TestStruct__fct__intref(ptr %test), !dbg !78
  store %"__TestStruct__long___rt_string::String_int" %1, ptr %res, align 8, !dbg !78
  call void @llvm.dbg.declare(metadata ptr %res, metadata !79, metadata !DIExpression()), !dbg !78
  store %"__TestStruct__long___rt_string::String_int" %1, ptr %res, align 8, !dbg !78
  %lng_addr = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %res, i32 0, i32 0, !dbg !90
  %2 = load i64, ptr %lng_addr, align 8, !dbg !90
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %2), !dbg !90
  %4 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %res, i32 0, i32 1, !dbg !91
  %5 = call ptr @_mf__String__string__getRaw(ptr %4), !dbg !91
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5), !dbg !91
  %i_addr = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %res, i32 0, i32 2, !dbg !92
  %7 = load i32, ptr %i_addr, align 4, !dbg !92
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %7), !dbg !92
  %9 = load i32, ptr %result, align 4, !dbg !92
  ret i32 %9, !dbg !92
}

declare i32 @printf(ptr noundef, ...)

declare ptr @_mf__String__string__getRaw(ptr)

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!16, !17, !18, !19, !20, !21, !22}
!llvm.ident = !{!23}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "anon.string.0", linkageName: "anon.string.0", scope: !2, file: !7, line: 8, type: !15, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C11, file: !3, producer: "spice version dev", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "C:\\Users\\Marc\\Documents\\JustForFunGitHubClonesFast\\spice\\cmake-build-debug\\test\\test-files\\irgenerator\\debug-info\\success-dbg-info-simple\\source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dbg-info-simple")
!4 = !{!0, !5, !9, !12}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !7, line: 15, type: !8, isLocal: true, isDefinition: true)
!7 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dbg-info-simple")
!8 = !DIStringType(name: "printf.str.0", size: 80)
!9 = !DIGlobalVariableExpression(var: !10, expr: !DIExpression())
!10 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !7, line: 16, type: !11, isLocal: true, isDefinition: true)
!11 = !DIStringType(name: "printf.str.1", size: 96)
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !7, line: 17, type: !14, isLocal: true, isDefinition: true)
!14 = !DIStringType(name: "printf.str.2", size: 72)
!15 = !DIStringType(name: "anon.string.0", size: 96)
!16 = !{i32 7, !"Dwarf Version", i32 4}
!17 = !{i32 2, !"Debug Info Version", i32 3}
!18 = !{i32 1, !"wchar_size", i32 4}
!19 = !{i32 8, !"PIC Level", i32 2}
!20 = !{i32 7, !"PIE Level", i32 0}
!21 = !{i32 7, !"uwtable", i32 2}
!22 = !{i32 7, !"frame-pointer", i32 2}
!23 = !{!"spice version dev"}
!24 = distinct !DISubprogram(name: "fct", linkageName: "_f__void__TestStruct__fct__intref", scope: !7, file: !7, line: 7, type: !25, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !43)
!25 = !DISubroutineType(types: !26)
!26 = !{!27, !42}
!27 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !28, identifier: "__TestStruct__long___rt_string::String_int")
!28 = !{!29, !31, !40}
!29 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !27, file: !7, line: 2, baseType: !30, size: 64)
!30 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !27, file: !7, line: 3, baseType: !32, size: 192, align: 8, offset: 64)
!32 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !7, file: !7, line: 14, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !33, identifier: "__String__charptr_long_long")
!33 = !{!34, !37, !39}
!34 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !32, file: !7, line: 15, baseType: !35, size: 64)
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !36, size: 64)
!36 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!37 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !32, file: !7, line: 16, baseType: !38, size: 64, offset: 64)
!38 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!39 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !32, file: !7, line: 17, baseType: !38, size: 64, offset: 128)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !27, file: !7, line: 4, baseType: !41, size: 32, offset: 256)
!41 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!42 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !41, size: 64)
!43 = !{}
!44 = !DILocalVariable(name: "result", scope: !24, file: !7, line: 7, type: !45)
!45 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !46, identifier: "__TestStruct__long___rt_string::String_int")
!46 = !{!47, !48, !54}
!47 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !45, file: !7, line: 2, baseType: !30, size: 64)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !45, file: !7, line: 3, baseType: !49, size: 192, align: 8, offset: 64)
!49 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !7, file: !7, line: 14, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !50, identifier: "__String__charptr_long_long")
!50 = !{!51, !52, !53}
!51 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !49, file: !7, line: 15, baseType: !35, size: 64)
!52 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !49, file: !7, line: 16, baseType: !38, size: 64, offset: 64)
!53 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !49, file: !7, line: 17, baseType: !38, size: 64, offset: 128)
!54 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !45, file: !7, line: 4, baseType: !41, size: 32, offset: 256)
!55 = !DILocation(line: 7, column: 1, scope: !24)
!56 = !DILocalVariable(name: "ref", arg: 1, scope: !24, file: !7, line: 7, type: !42)
!57 = !DILocation(line: 8, column: 44, scope: !24)
!58 = !DILocation(line: 8, column: 60, scope: !24)
!59 = !DILocalVariable(name: "ts", scope: !24, file: !7, line: 8, type: !60)
!60 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !61, identifier: "__TestStruct__long___rt_string::String_int")
!61 = !{!62, !63, !69}
!62 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !60, file: !7, line: 2, baseType: !30, size: 64)
!63 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !60, file: !7, line: 3, baseType: !64, size: 192, align: 8, offset: 64)
!64 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !7, file: !7, line: 14, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !65, identifier: "__String__charptr_long_long")
!65 = !{!66, !67, !68}
!66 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !64, file: !7, line: 15, baseType: !35, size: 64)
!67 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !64, file: !7, line: 16, baseType: !38, size: 64, offset: 64)
!68 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !64, file: !7, line: 17, baseType: !38, size: 64, offset: 128)
!69 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !60, file: !7, line: 4, baseType: !41, size: 32, offset: 256)
!70 = !DILocation(line: 9, column: 12, scope: !24)
!71 = distinct !DISubprogram(name: "main", linkageName: "main", scope: !7, file: !7, line: 12, type: !72, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !43)
!72 = !DISubroutineType(types: !73)
!73 = !{!41}
!74 = !DILocalVariable(name: "result", scope: !71, file: !7, line: 12, type: !41)
!75 = !DILocation(line: 12, column: 1, scope: !71)
!76 = !DILocalVariable(name: "test", scope: !71, file: !7, line: 13, type: !41)
!77 = !DILocation(line: 13, column: 16, scope: !71)
!78 = !DILocation(line: 14, column: 32, scope: !71)
!79 = !DILocalVariable(name: "res", scope: !71, file: !7, line: 14, type: !80)
!80 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !81, identifier: "__TestStruct__long___rt_string::String_int")
!81 = !{!82, !83, !89}
!82 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !80, file: !7, line: 2, baseType: !30, size: 64)
!83 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !80, file: !7, line: 3, baseType: !84, size: 192, align: 8, offset: 64)
!84 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !7, file: !7, line: 14, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !85, identifier: "__String__charptr_long_long")
!85 = !{!86, !87, !88}
!86 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !84, file: !7, line: 15, baseType: !35, size: 64)
!87 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !84, file: !7, line: 16, baseType: !38, size: 64, offset: 64)
!88 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !84, file: !7, line: 17, baseType: !38, size: 64, offset: 128)
!89 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !80, file: !7, line: 4, baseType: !41, size: 32, offset: 256)
!90 = !DILocation(line: 15, column: 26, scope: !71)
!91 = !DILocation(line: 16, column: 28, scope: !71)
!92 = !DILocation(line: 17, column: 25, scope: !71)
