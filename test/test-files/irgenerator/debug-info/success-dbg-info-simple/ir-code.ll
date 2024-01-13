; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.TestStruct = type { i64, %struct.String, i32 }
%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1, !dbg !0
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Long: %d\0A\00", align 1, !dbg !5
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 1, !dbg !9
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1, !dbg !12

; Function Attrs: norecurse
define private void @_ZN10TestStruct4dtorEv(ptr noundef nonnull align 8 dereferenceable(40) %0) #0 !dbg !24 {
  %this = alloca ptr, align 8
  call void @llvm.dbg.declare(metadata ptr %this, metadata !45, metadata !DIExpression()), !dbg !47
  store ptr %0, ptr %this, align 8, !dbg !47
  %2 = load ptr, ptr %this, align 8, !dbg !47
  %3 = getelementptr inbounds %struct.TestStruct, ptr %2, i32 0, i32 1, !dbg !47
  call void @_ZN6String4dtorEv(ptr %3), !dbg !47
  ret void, !dbg !47
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare void @_ZN6String4dtorEv(ptr)

define private %struct.TestStruct @_Z3fctRi(ptr %0) !dbg !48 {
  %result = alloca %struct.TestStruct, align 8
  %ref = alloca ptr, align 8
  %2 = alloca %struct.String, align 8
  %ts = alloca %struct.TestStruct, align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !52, metadata !DIExpression()), !dbg !53
  call void @llvm.dbg.declare(metadata ptr %ref, metadata !54, metadata !DIExpression()), !dbg !55
  store ptr %0, ptr %ref, align 8, !dbg !55
  call void @_ZN6String4ctorEPc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.0), !dbg !56
  store i64 6, ptr %ts, align 8, !dbg !57
  %3 = load %struct.String, ptr %2, align 8, !dbg !57
  %4 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1, !dbg !57
  store %struct.String %3, ptr %4, align 8, !dbg !57
  %5 = load ptr, ptr %ref, align 8, !dbg !57
  %6 = load i32, ptr %5, align 4, !dbg !57
  %7 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 2, !dbg !57
  call void @llvm.dbg.declare(metadata ptr %ts, metadata !58, metadata !DIExpression()), !dbg !59
  store i32 %6, ptr %7, align 4, !dbg !57
  %8 = load %struct.TestStruct, ptr %ts, align 8, !dbg !60
  ret %struct.TestStruct %8, !dbg !60
}

declare void @_ZN6String4ctorEPc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #2 !dbg !61 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %res = alloca %struct.TestStruct, align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !64, metadata !DIExpression()), !dbg !65
  store i32 0, ptr %result, align 4, !dbg !65
  call void @llvm.dbg.declare(metadata ptr %test, metadata !66, metadata !DIExpression()), !dbg !67
  store i32 987654, ptr %test, align 4, !dbg !68
  %1 = call %struct.TestStruct @_Z3fctRi(ptr %test), !dbg !69
  call void @llvm.dbg.declare(metadata ptr %res, metadata !70, metadata !DIExpression()), !dbg !71
  store %struct.TestStruct %1, ptr %res, align 8, !dbg !69
  %lng_addr = getelementptr inbounds %struct.TestStruct, ptr %res, i32 0, i32 0, !dbg !72
  %2 = load i64, ptr %lng_addr, align 8, !dbg !72
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %2), !dbg !72
  %4 = getelementptr inbounds %struct.TestStruct, ptr %res, i32 0, i32 1, !dbg !73
  %5 = call ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %4), !dbg !73
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5), !dbg !73
  %i_addr = getelementptr inbounds %struct.TestStruct, ptr %res, i32 0, i32 2, !dbg !74
  %7 = load i32, ptr %i_addr, align 4, !dbg !74
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %7), !dbg !74
  call void @_ZN10TestStruct4dtorEv(ptr %res), !dbg !74
  %9 = load i32, ptr %result, align 4, !dbg !74
  ret i32 %9, !dbg !74
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #3

declare ptr @_ZN6String6getRawEv(ptr)

attributes #0 = { norecurse }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nofree nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!16, !17, !18, !19, !20, !21, !22}
!llvm.ident = !{!23}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "anon.string.0", linkageName: "anon.string.0", scope: !2, file: !7, line: 8, type: !15, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_20, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
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
!20 = !{i32 7, !"PIE Level", i32 2}
!21 = !{i32 7, !"uwtable", i32 2}
!22 = !{i32 7, !"frame-pointer", i32 2}
!23 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!24 = distinct !DISubprogram(name: "dtor", linkageName: "_ZN10TestStruct4dtorEv", scope: !7, file: !7, line: 1, type: !25, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !44)
!25 = !DISubroutineType(types: !26)
!26 = !{!27, !28}
!27 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !29, identifier: "struct.TestStruct")
!29 = !{!30, !32, !42}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !28, file: !7, line: 2, baseType: !31, size: 64)
!31 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !28, file: !7, line: 3, baseType: !33, size: 192, align: 8, offset: 64)
!33 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !34, file: !34, line: 20, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !35, identifier: "struct.String")
!34 = !DIFile(filename: "string_rt.spice", directory: "C:\\Users\\Marc\\Documents\\JustForFunGitHubClonesFast\\spice\\std\\runtime")
!35 = !{!36, !39, !41}
!36 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !33, file: !34, line: 21, baseType: !37, size: 64)
!37 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !38, size: 64)
!38 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!39 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !33, file: !34, line: 22, baseType: !40, size: 64, offset: 64)
!40 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!41 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !33, file: !34, line: 23, baseType: !40, size: 64, offset: 128)
!42 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !28, file: !7, line: 4, baseType: !43, size: 32, offset: 256)
!43 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!44 = !{}
!45 = !DILocalVariable(name: "this", arg: 1, scope: !24, file: !7, line: 1, type: !46)
!46 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64)
!47 = !DILocation(line: 1, column: 1, scope: !24)
!48 = distinct !DISubprogram(name: "fct", linkageName: "_Z3fctRi", scope: !7, file: !7, line: 7, type: !49, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !44)
!49 = !DISubroutineType(types: !50)
!50 = !{!28, !51}
!51 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !43, size: 64)
!52 = !DILocalVariable(name: "result", scope: !48, file: !7, line: 7, type: !28)
!53 = !DILocation(line: 7, column: 1, scope: !48)
!54 = !DILocalVariable(name: "ref", arg: 1, scope: !48, file: !7, line: 7, type: !51)
!55 = !DILocation(line: 7, column: 19, scope: !48)
!56 = !DILocation(line: 8, column: 44, scope: !48)
!57 = !DILocation(line: 8, column: 60, scope: !48)
!58 = !DILocalVariable(name: "ts", scope: !48, file: !7, line: 8, type: !28)
!59 = !DILocation(line: 8, column: 5, scope: !48)
!60 = !DILocation(line: 9, column: 12, scope: !48)
!61 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !7, file: !7, line: 12, type: !62, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !44)
!62 = !DISubroutineType(types: !63)
!63 = !{!43}
!64 = !DILocalVariable(name: "result", scope: !61, file: !7, line: 12, type: !43)
!65 = !DILocation(line: 12, column: 1, scope: !61)
!66 = !DILocalVariable(name: "test", scope: !61, file: !7, line: 13, type: !43)
!67 = !DILocation(line: 13, column: 5, scope: !61)
!68 = !DILocation(line: 13, column: 16, scope: !61)
!69 = !DILocation(line: 14, column: 32, scope: !61)
!70 = !DILocalVariable(name: "res", scope: !61, file: !7, line: 14, type: !28)
!71 = !DILocation(line: 14, column: 5, scope: !61)
!72 = !DILocation(line: 15, column: 26, scope: !61)
!73 = !DILocation(line: 16, column: 28, scope: !61)
!74 = !DILocation(line: 17, column: 25, scope: !61)
