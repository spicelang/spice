; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"__TestStruct__long___rt_string::String_int" = type { i64, %__String__charptr_long_long, i32 }
%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Long: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1

define private %"__TestStruct__long___rt_string::String_int" @_f__void__TestStruct__fct__intref(ptr %0) !dbg !6 {
  %result = alloca %"__TestStruct__long___rt_string::String_int", align 8
  %ref = alloca ptr, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %ts = alloca %"__TestStruct__long___rt_string::String_int", align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !21, metadata !DIExpression()), !dbg !22
  call void @llvm.dbg.declare(metadata ptr %ref, metadata !23, metadata !DIExpression()), !dbg !22
  store ptr %0, ptr %ref, align 8, !dbg !22
  call void @_mp__String__void__ctor__string(ptr %2, ptr @anon.string.0), !dbg !24
  %3 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %ts, i32 0, i32 0, !dbg !25
  store i64 6, ptr %3, align 8, !dbg !25
  %4 = load %__String__charptr_long_long, ptr %2, align 8, !dbg !25
  %5 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %ts, i32 0, i32 1, !dbg !25
  store %__String__charptr_long_long %4, ptr %5, align 8, !dbg !25
  %6 = load ptr, ptr %ref, align 8, !dbg !25
  %7 = load i32, ptr %6, align 4, !dbg !25
  %8 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %ts, i32 0, i32 2, !dbg !25
  call void @llvm.dbg.declare(metadata ptr %ts, metadata !26, metadata !DIExpression()), !dbg !25
  store i32 %7, ptr %8, align 4, !dbg !25
  %9 = load %"__TestStruct__long___rt_string::String_int", ptr %ts, align 8, !dbg !27
  ret %"__TestStruct__long___rt_string::String_int" %9, !dbg !27
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #0

declare void @_mp__String__void__ctor__string(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 !dbg !28 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %res = alloca %"__TestStruct__long___rt_string::String_int", align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !31, metadata !DIExpression()), !dbg !32
  store i32 0, ptr %result, align 4, !dbg !32
  call void @llvm.dbg.declare(metadata ptr %test, metadata !33, metadata !DIExpression()), !dbg !34
  store i32 987654, ptr %test, align 4, !dbg !34
  %1 = call %"__TestStruct__long___rt_string::String_int" @_f__void__TestStruct__fct__intref(ptr %test), !dbg !35
  store %"__TestStruct__long___rt_string::String_int" %1, ptr %res, align 8, !dbg !35
  call void @llvm.dbg.declare(metadata ptr %res, metadata !36, metadata !DIExpression()), !dbg !35
  store %"__TestStruct__long___rt_string::String_int" %1, ptr %res, align 8, !dbg !35
  %lng_addr = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %res, i32 0, i32 0, !dbg !37
  %2 = load i64, ptr %lng_addr, align 8, !dbg !37
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %2), !dbg !37
  %4 = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %res, i32 0, i32 1, !dbg !38
  %5 = call ptr @_mf__String__string__getRaw(ptr %4), !dbg !38
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %5), !dbg !38
  %i_addr = getelementptr inbounds %"__TestStruct__long___rt_string::String_int", ptr %res, i32 0, i32 2, !dbg !39
  %7 = load i32, ptr %i_addr, align 4, !dbg !39
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %7), !dbg !39
  %9 = load i32, ptr %result, align 4, !dbg !39
  ret i32 %9, !dbg !39
}

declare i32 @printf(ptr noundef, ...)

declare ptr @_mf__String__string__getRaw(ptr)

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}
!llvm.dbg.cu = !{!4}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 1, !"PIC Level", i32 2}
!3 = !{!"spice version dev"}
!4 = distinct !DICompileUnit(language: DW_LANG_C11, file: !5, producer: "spice version dev", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!5 = !DIFile(filename: "C:\\Users\\Marc\\Documents\\JustForFunGitHubClonesFast\\spice\\cmake-build-debug\\test\\test-files\\irgenerator\\debug-info\\success-dbg-info-simple\\source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dbg-info-simple")
!6 = distinct !DISubprogram(name: "fct", linkageName: "_f__void__TestStruct__fct__intref", scope: !7, file: !7, line: 7, type: !8, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !4, retainedNodes: !20)
!7 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dbg-info-simple")
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !19}
!10 = !DICompositeType(tag: DW_TAG_structure_type, name: "TestStruct", scope: !7, file: !7, line: 1, align: 8, flags: DIFlagTypePassByValue, elements: !11)
!11 = !{!12, !13, !18}
!12 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!13 = !DICompositeType(tag: DW_TAG_structure_type, name: "String", scope: !7, file: !7, line: 15, align: 8, flags: DIFlagTypePassByValue, elements: !14)
!14 = !{!15, !17, !17}
!15 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 64)
!16 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!17 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!20 = !{}
!21 = !DILocalVariable(name: "result", scope: !6, file: !7, line: 7, type: !10)
!22 = !DILocation(line: 7, column: 1, scope: !6)
!23 = !DILocalVariable(name: "ref", arg: 1, scope: !6, file: !7, line: 7, type: !19)
!24 = !DILocation(line: 8, column: 44, scope: !6)
!25 = !DILocation(line: 8, column: 60, scope: !6)
!26 = !DILocalVariable(name: "ts", scope: !6, file: !7, line: 8, type: !10)
!27 = !DILocation(line: 9, column: 12, scope: !6)
!28 = distinct !DISubprogram(name: "main", linkageName: "main", scope: !7, file: !7, line: 12, type: !29, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !4, retainedNodes: !20)
!29 = !DISubroutineType(types: !30)
!30 = !{!18}
!31 = !DILocalVariable(name: "result", scope: !28, file: !7, line: 12, type: !18)
!32 = !DILocation(line: 12, column: 1, scope: !28)
!33 = !DILocalVariable(name: "test", scope: !28, file: !7, line: 13, type: !18)
!34 = !DILocation(line: 13, column: 16, scope: !28)
!35 = !DILocation(line: 14, column: 32, scope: !28)
!36 = !DILocalVariable(name: "res", scope: !28, file: !7, line: 14, type: !10)
!37 = !DILocation(line: 15, column: 26, scope: !28)
!38 = !DILocation(line: 16, column: 28, scope: !28)
!39 = !DILocation(line: 17, column: 25, scope: !28)
