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

define private %struct.TestStruct @_Z3fctRi(ptr %0) !dbg !24 {
  %result = alloca %struct.TestStruct, align 8
  %ref = alloca ptr, align 8
  %2 = alloca %struct.String, align 8
  %ts = alloca %struct.TestStruct, align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !45, metadata !DIExpression()), !dbg !46
  call void @llvm.dbg.declare(metadata ptr %ref, metadata !47, metadata !DIExpression()), !dbg !46
  store ptr %0, ptr %ref, align 8, !dbg !46
  call void @_ZN6String4ctorEPc(ptr %2, ptr @anon.string.0), !dbg !48
  %3 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 0, !dbg !49
  store i64 6, ptr %3, align 8, !dbg !49
  %4 = load %struct.String, ptr %2, align 8, !dbg !49
  %5 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1, !dbg !49
  store %struct.String %4, ptr %5, align 8, !dbg !49
  %6 = load ptr, ptr %ref, align 8, !dbg !49
  %7 = load i32, ptr %6, align 4, !dbg !49
  %8 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 2, !dbg !49
  call void @llvm.dbg.declare(metadata ptr %ts, metadata !50, metadata !DIExpression()), !dbg !51
  store i32 %7, ptr %8, align 4, !dbg !49
  %9 = load %struct.TestStruct, ptr %ts, align 8, !dbg !52
  ret %struct.TestStruct %9, !dbg !52
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #0

declare void @_ZN6String4ctorEPc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 !dbg !53 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %res = alloca %struct.TestStruct, align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !56, metadata !DIExpression()), !dbg !57
  store i32 0, ptr %result, align 4, !dbg !57
  call void @llvm.dbg.declare(metadata ptr %test, metadata !58, metadata !DIExpression()), !dbg !59
  store i32 987654, ptr %test, align 4, !dbg !60
  %1 = call %struct.TestStruct @_Z3fctRi(ptr %test), !dbg !61
  store %struct.TestStruct %1, ptr %res, align 8, !dbg !61
  call void @llvm.dbg.declare(metadata ptr %res, metadata !62, metadata !DIExpression()), !dbg !63
  store %struct.TestStruct %1, ptr %res, align 8, !dbg !61
  %lng_addr = getelementptr inbounds %struct.TestStruct, ptr %res, i32 0, i32 0, !dbg !64
  %2 = load i64, ptr %lng_addr, align 8, !dbg !64
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %2), !dbg !64
  %4 = getelementptr inbounds %struct.TestStruct, ptr %res, i32 0, i32 1, !dbg !65
  %5 = call ptr @_ZN6String6getRawEv(ptr %4), !dbg !65
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5), !dbg !65
  %i_addr = getelementptr inbounds %struct.TestStruct, ptr %res, i32 0, i32 2, !dbg !66
  %7 = load i32, ptr %i_addr, align 4, !dbg !66
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %7), !dbg !66
  %9 = load i32, ptr %result, align 4, !dbg !66
  ret i32 %9, !dbg !66
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

declare ptr @_ZN6String6getRawEv(ptr)

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!16, !17, !18, !19, !20, !21, !22}
!llvm.ident = !{!23}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "anon.string.0", linkageName: "anon.string.0", scope: !2, file: !7, line: 8, type: !15, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: 32768, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
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
!23 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!24 = distinct !DISubprogram(name: "fct", linkageName: "_Z3fctRi", scope: !7, file: !7, line: 7, type: !25, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !44)
!25 = !DISubroutineType(types: !26)
!26 = !{!27, !43}
!27 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, size: 320, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !28, identifier: "struct.TestStruct")
!28 = !{!29, !31, !41}
!29 = !DIDerivedType(tag: DW_TAG_member, name: "lng", scope: !27, file: !7, line: 2, baseType: !30, size: 64)
!30 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "str", scope: !27, file: !7, line: 3, baseType: !32, size: 192, align: 8, offset: 64)
!32 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !33, file: !33, line: 15, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !34, identifier: "struct.String")
!33 = !DIFile(filename: "string_rt.spice", directory: "C:\\Users\\Marc\\Documents\\JustForFunGitHubClonesFast\\spice\\std\\runtime")
!34 = !{!35, !38, !40}
!35 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !32, file: !33, line: 16, baseType: !36, size: 64)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !37, size: 64)
!37 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!38 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !32, file: !33, line: 17, baseType: !39, size: 64, offset: 64)
!39 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !32, file: !33, line: 18, baseType: !39, size: 64, offset: 128)
!41 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !27, file: !7, line: 4, baseType: !42, size: 32, offset: 256)
!42 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!43 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !42, size: 64)
!44 = !{}
!45 = !DILocalVariable(name: "result", scope: !24, file: !7, line: 7, type: !27)
!46 = !DILocation(line: 7, column: 1, scope: !24)
!47 = !DILocalVariable(name: "ref", arg: 1, scope: !24, file: !7, line: 7, type: !43)
!48 = !DILocation(line: 8, column: 44, scope: !24)
!49 = !DILocation(line: 8, column: 60, scope: !24)
!50 = !DILocalVariable(name: "ts", scope: !24, file: !7, line: 8, type: !27)
!51 = !DILocation(line: 8, column: 5, scope: !24)
!52 = !DILocation(line: 9, column: 12, scope: !24)
!53 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !7, file: !7, line: 12, type: !54, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !44)
!54 = !DISubroutineType(types: !55)
!55 = !{!42}
!56 = !DILocalVariable(name: "result", scope: !53, file: !7, line: 12, type: !42)
!57 = !DILocation(line: 12, column: 1, scope: !53)
!58 = !DILocalVariable(name: "test", scope: !53, file: !7, line: 13, type: !42)
!59 = !DILocation(line: 13, column: 5, scope: !53)
!60 = !DILocation(line: 13, column: 16, scope: !53)
!61 = !DILocation(line: 14, column: 32, scope: !53)
!62 = !DILocalVariable(name: "res", scope: !53, file: !7, line: 14, type: !27)
!63 = !DILocation(line: 14, column: 5, scope: !53)
!64 = !DILocation(line: 15, column: 26, scope: !53)
!65 = !DILocation(line: 16, column: 28, scope: !53)
!66 = !DILocation(line: 17, column: 25, scope: !53)
