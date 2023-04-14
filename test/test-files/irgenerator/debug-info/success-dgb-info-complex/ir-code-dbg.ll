; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Vector__intptr_long_long_int = type { ptr, i64, i64, i32 }
%"__int__VectorIterator__std/data/vector::Vector<int>ref_long" = type { ptr, i64 }
%__long_intref__Pair__long_intref = type { i64, ptr }

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.getSize() == 3' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !6 {
  %result = alloca i32, align 4
  %vi = alloca %__int__Vector__intptr_long_long_int, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %it = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %pair = alloca %__long_intref__Pair__long_intref, align 8
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i1, align 1
  call void @llvm.dbg.declare(metadata ptr %result, metadata !12, metadata !DIExpression()), !dbg !13
  store i32 0, ptr %result, align 4, !dbg !13
  call void @llvm.dbg.declare(metadata ptr %vi, metadata !14, metadata !DIExpression()), !dbg !20
  call void @_mp__Vector_int__void__ctor(ptr %vi), !dbg !20
  store i32 123, ptr %1, align 4, !dbg !21
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %1), !dbg !21
  store i32 4321, ptr %2, align 4, !dbg !22
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %2), !dbg !22
  store i32 9876, ptr %3, align 4, !dbg !23
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %3), !dbg !23
  %8 = call i64 @_mf__Vector_int__long__getSize(ptr %vi), !dbg !24
  %9 = icmp eq i64 %8, 3, !dbg !25
  br i1 %9, label %assert.exit.L9, label %assert.then.L9, !dbg !25, !prof !26

assert.then.L9:                                   ; preds = %0
  %10 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !25
  call void @exit(i32 1), !dbg !25
  unreachable, !dbg !25

assert.exit.L9:                                   ; preds = %0
  %11 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !27
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %11, ptr %it, align 8, !dbg !27
  call void @llvm.dbg.declare(metadata ptr %it, metadata !28, metadata !DIExpression()), !dbg !27
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %11, ptr %it, align 8, !dbg !27
  %12 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !32
  br i1 %12, label %assert.exit.L13, label %assert.then.L13, !dbg !32, !prof !26

assert.then.L13:                                  ; preds = %assert.exit.L9
  %13 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !32
  call void @exit(i32 1), !dbg !32
  unreachable, !dbg !32

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %14 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !33
  %15 = load i32, ptr %14, align 4, !dbg !34
  %16 = icmp eq i32 %15, 123, !dbg !34
  br i1 %16, label %assert.exit.L14, label %assert.then.L14, !dbg !34, !prof !26

assert.then.L14:                                  ; preds = %assert.exit.L13
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !34
  call void @exit(i32 1), !dbg !34
  unreachable, !dbg !34

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %18 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !35
  %19 = load i32, ptr %18, align 4, !dbg !36
  %20 = icmp eq i32 %19, 123, !dbg !36
  br i1 %20, label %assert.exit.L15, label %assert.then.L15, !dbg !36, !prof !26

assert.then.L15:                                  ; preds = %assert.exit.L14
  %21 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !36
  call void @exit(i32 1), !dbg !36
  unreachable, !dbg !36

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !37
  %22 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !38
  %23 = load i32, ptr %22, align 4, !dbg !39
  %24 = icmp eq i32 %23, 4321, !dbg !39
  br i1 %24, label %assert.exit.L17, label %assert.then.L17, !dbg !39, !prof !26

assert.then.L17:                                  ; preds = %assert.exit.L15
  %25 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !39
  call void @exit(i32 1), !dbg !39
  unreachable, !dbg !39

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %26 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !40
  br i1 %26, label %assert.exit.L18, label %assert.then.L18, !dbg !40, !prof !26

assert.then.L18:                                  ; preds = %assert.exit.L17
  %27 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !40
  call void @exit(i32 1), !dbg !40
  unreachable, !dbg !40

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !41
  %28 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr %it), !dbg !42
  store %__long_intref__Pair__long_intref %28, ptr %pair, align 8, !dbg !42
  call void @llvm.dbg.declare(metadata ptr %pair, metadata !43, metadata !DIExpression()), !dbg !42
  store %__long_intref__Pair__long_intref %28, ptr %pair, align 8, !dbg !42
  %29 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr %pair), !dbg !46
  %30 = load i64, ptr %29, align 8, !dbg !47
  %31 = icmp eq i64 %30, 2, !dbg !47
  br i1 %31, label %assert.exit.L21, label %assert.then.L21, !dbg !47, !prof !26

assert.then.L21:                                  ; preds = %assert.exit.L18
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !47
  call void @exit(i32 1), !dbg !47
  unreachable, !dbg !47

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %33 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr %pair), !dbg !48
  %34 = load i32, ptr %33, align 4, !dbg !49
  %35 = icmp eq i32 %34, 9876, !dbg !49
  br i1 %35, label %assert.exit.L22, label %assert.then.L22, !dbg !49, !prof !26

assert.then.L22:                                  ; preds = %assert.exit.L21
  %36 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !49
  call void @exit(i32 1), !dbg !49
  unreachable, !dbg !49

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !50
  %37 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !51
  %38 = xor i1 %37, true, !dbg !51
  store i1 %38, ptr %4, align 1, !dbg !51
  br i1 %38, label %assert.exit.L24, label %assert.then.L24, !dbg !51, !prof !26

assert.then.L24:                                  ; preds = %assert.exit.L22
  %39 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !51
  call void @exit(i32 1), !dbg !51
  unreachable, !dbg !51

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %5, align 4, !dbg !52
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %5), !dbg !52
  store i32 -99, ptr %6, align 4, !dbg !53
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %6), !dbg !53
  %40 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !54
  br i1 %40, label %assert.exit.L29, label %assert.then.L29, !dbg !54, !prof !26

assert.then.L29:                                  ; preds = %assert.exit.L24
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !54
  call void @exit(i32 1), !dbg !54
  unreachable, !dbg !54

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr %it, i32 3), !dbg !55
  %42 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !56
  %43 = load i32, ptr %42, align 4, !dbg !57
  %44 = icmp eq i32 %43, 123, !dbg !57
  br i1 %44, label %assert.exit.L33, label %assert.then.L33, !dbg !57, !prof !26

assert.then.L33:                                  ; preds = %assert.exit.L29
  %45 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !57
  call void @exit(i32 1), !dbg !57
  unreachable, !dbg !57

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %46 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !58
  br i1 %46, label %assert.exit.L34, label %assert.then.L34, !dbg !58, !prof !26

assert.then.L34:                                  ; preds = %assert.exit.L33
  %47 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %48 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !59
  call void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr %it), !dbg !59
  %49 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !60
  %50 = load i32, ptr %49, align 4, !dbg !61
  %51 = icmp eq i32 %50, 4321, !dbg !61
  br i1 %51, label %assert.exit.L36, label %assert.then.L36, !dbg !61, !prof !26

assert.then.L36:                                  ; preds = %assert.exit.L34
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !61
  call void @exit(i32 1), !dbg !61
  unreachable, !dbg !61

assert.exit.L36:                                  ; preds = %assert.exit.L34
  %53 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !62
  call void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr %it), !dbg !62
  %54 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !63
  %55 = load i32, ptr %54, align 4, !dbg !64
  %56 = icmp eq i32 %55, 123, !dbg !64
  br i1 %56, label %assert.exit.L38, label %assert.then.L38, !dbg !64, !prof !26

assert.then.L38:                                  ; preds = %assert.exit.L36
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !64
  call void @exit(i32 1), !dbg !64
  unreachable, !dbg !64

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr %it, i32 4), !dbg !65
  %58 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !66
  %59 = load i32, ptr %58, align 4, !dbg !67
  %60 = icmp eq i32 %59, -99, !dbg !67
  br i1 %60, label %assert.exit.L40, label %assert.then.L40, !dbg !67, !prof !26

assert.then.L40:                                  ; preds = %assert.exit.L38
  %61 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !67
  call void @exit(i32 1), !dbg !67
  unreachable, !dbg !67

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !68
  %62 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !69
  %63 = xor i1 %62, true, !dbg !69
  store i1 %63, ptr %7, align 1, !dbg !69
  br i1 %63, label %assert.exit.L42, label %assert.then.L42, !dbg !69, !prof !26

assert.then.L42:                                  ; preds = %assert.exit.L40
  %64 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !69
  call void @exit(i32 1), !dbg !69
  unreachable, !dbg !69

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %65 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !70
  %66 = load i32, ptr %result, align 4, !dbg !70
  ret i32 %66, !dbg !70
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare void @_mp__Vector_int__void__ctor(ptr)

declare void @_mp__Vector_int__void__pushBack__intref(ptr, ptr)

declare i64 @_mf__Vector_int__long__getSize(ptr)

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

declare %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr)

declare i1 @_mf__VectorIterator_int__bool__isValid(ptr)

declare ptr @_mf__VectorIterator_int__intref__get(ptr)

declare void @_mp__VectorIterator_int__void__next(ptr)

declare %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr)

declare ptr @_mf__Pair_long_intref__longref__getFirst(ptr)

declare ptr @_mf__Pair_long_intref__intref__getSecond(ptr)

declare void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr, i32)

declare void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr)

declare void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr)

declare void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr, i32)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}
!llvm.dbg.cu = !{!4}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 1, !"PIC Level", i32 2}
!3 = !{!"spice version dev"}
!4 = distinct !DICompileUnit(language: DW_LANG_C11, file: !5, producer: "spice version dev", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!5 = !DIFile(filename: "C:\\Users\\I516467\\Documents\\JustForFunGitHubClones\\spice\\cmake-build-debug\\test\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex\\source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!6 = distinct !DISubprogram(name: "main", linkageName: "main", scope: !7, file: !7, line: 3, type: !8, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !4, retainedNodes: !11)
!7 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!8 = !DISubroutineType(types: !9)
!9 = !{!10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !{}
!12 = !DILocalVariable(name: "result", scope: !6, file: !7, line: 3, type: !10)
!13 = !DILocation(line: 3, column: 1, scope: !6)
!14 = !DILocalVariable(name: "vi", scope: !6, file: !7, line: 5, type: !15)
!15 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !7, file: !7, line: 25, align: 8, flags: DIFlagTypePassByValue, elements: !16)
!16 = !{!17, !18, !18, !19}
!17 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!18 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!19 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!20 = !DILocation(line: 5, column: 22, scope: !6)
!21 = !DILocation(line: 6, column: 17, scope: !6)
!22 = !DILocation(line: 7, column: 17, scope: !6)
!23 = !DILocation(line: 8, column: 17, scope: !6)
!24 = !DILocation(line: 9, column: 12, scope: !6)
!25 = !DILocation(line: 9, column: 28, scope: !6)
!26 = !{!"branch_weights", i32 2000, i32 1}
!27 = !DILocation(line: 12, column: 22, scope: !6)
!28 = !DILocalVariable(name: "it", scope: !6, file: !7, line: 12, type: !29)
!29 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !7, file: !7, line: 11, align: 8, flags: DIFlagTypePassByValue, elements: !30)
!30 = !{!31, !18}
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !15, size: 64)
!32 = !DILocation(line: 13, column: 12, scope: !6)
!33 = !DILocation(line: 14, column: 12, scope: !6)
!34 = !DILocation(line: 14, column: 24, scope: !6)
!35 = !DILocation(line: 15, column: 12, scope: !6)
!36 = !DILocation(line: 15, column: 24, scope: !6)
!37 = !DILocation(line: 16, column: 5, scope: !6)
!38 = !DILocation(line: 17, column: 12, scope: !6)
!39 = !DILocation(line: 17, column: 24, scope: !6)
!40 = !DILocation(line: 18, column: 12, scope: !6)
!41 = !DILocation(line: 19, column: 5, scope: !6)
!42 = !DILocation(line: 20, column: 16, scope: !6)
!43 = !DILocalVariable(name: "pair", scope: !6, file: !7, line: 20, type: !44)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !7, file: !7, line: 8, align: 8, flags: DIFlagTypePassByValue, elements: !45)
!45 = !{!18, !17}
!46 = !DILocation(line: 21, column: 12, scope: !6)
!47 = !DILocation(line: 21, column: 31, scope: !6)
!48 = !DILocation(line: 22, column: 12, scope: !6)
!49 = !DILocation(line: 22, column: 32, scope: !6)
!50 = !DILocation(line: 23, column: 5, scope: !6)
!51 = !DILocation(line: 24, column: 13, scope: !6)
!52 = !DILocation(line: 27, column: 17, scope: !6)
!53 = !DILocation(line: 28, column: 17, scope: !6)
!54 = !DILocation(line: 29, column: 12, scope: !6)
!55 = !DILocation(line: 32, column: 5, scope: !6)
!56 = !DILocation(line: 33, column: 12, scope: !6)
!57 = !DILocation(line: 33, column: 24, scope: !6)
!58 = !DILocation(line: 34, column: 12, scope: !6)
!59 = !DILocation(line: 35, column: 5, scope: !6)
!60 = !DILocation(line: 36, column: 12, scope: !6)
!61 = !DILocation(line: 36, column: 24, scope: !6)
!62 = !DILocation(line: 37, column: 5, scope: !6)
!63 = !DILocation(line: 38, column: 12, scope: !6)
!64 = !DILocation(line: 38, column: 24, scope: !6)
!65 = !DILocation(line: 39, column: 5, scope: !6)
!66 = !DILocation(line: 40, column: 12, scope: !6)
!67 = !DILocation(line: 40, column: 24, scope: !6)
!68 = !DILocation(line: 41, column: 5, scope: !6)
!69 = !DILocation(line: 42, column: 13, scope: !6)
!70 = !DILocation(line: 44, column: 5, scope: !6)
