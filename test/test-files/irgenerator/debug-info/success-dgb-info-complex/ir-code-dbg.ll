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
define dso_local i32 @main(i32 %0, ptr %1) #0 !dbg !6 {
  %result = alloca i32, align 4
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
  %vi = alloca %__int__Vector__intptr_long_long_int, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %it = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %pair = alloca %__long_intref__Pair__long_intref, align 8
  %6 = alloca i1, align 1
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i1, align 1
  call void @llvm.dbg.declare(metadata ptr %result, metadata !14, metadata !DIExpression()), !dbg !15
  store i32 0, ptr %result, align 4, !dbg !15
  call void @llvm.dbg.declare(metadata ptr %argc, metadata !16, metadata !DIExpression()), !dbg !15
  store i32 %0, ptr %argc, align 4, !dbg !15
  call void @llvm.dbg.declare(metadata ptr %argv, metadata !17, metadata !DIExpression()), !dbg !15
  store ptr %1, ptr %argv, align 8, !dbg !15
  call void @llvm.dbg.declare(metadata ptr %vi, metadata !18, metadata !DIExpression()), !dbg !24
  call void @_mp__Vector_int__void__ctor(ptr %vi), !dbg !24
  store i32 123, ptr %3, align 4, !dbg !25
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %3), !dbg !25
  store i32 4321, ptr %4, align 4, !dbg !26
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %4), !dbg !26
  store i32 9876, ptr %5, align 4, !dbg !27
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %5), !dbg !27
  %10 = call i64 @_mf__Vector_int__long__getSize(ptr %vi), !dbg !28
  %11 = icmp eq i64 %10, 3, !dbg !29
  br i1 %11, label %assert.exit.L9, label %assert.then.L9, !dbg !29, !prof !30

assert.then.L9:                                   ; preds = %2
  %12 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !29
  call void @exit(i32 1), !dbg !29
  unreachable, !dbg !29

assert.exit.L9:                                   ; preds = %2
  %13 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !31
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %13, ptr %it, align 8, !dbg !31
  call void @llvm.dbg.declare(metadata ptr %it, metadata !32, metadata !DIExpression()), !dbg !31
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %13, ptr %it, align 8, !dbg !31
  %14 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !36
  br i1 %14, label %assert.exit.L13, label %assert.then.L13, !dbg !36, !prof !30

assert.then.L13:                                  ; preds = %assert.exit.L9
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !36
  call void @exit(i32 1), !dbg !36
  unreachable, !dbg !36

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %16 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !37
  %17 = load i32, ptr %16, align 4, !dbg !38
  %18 = icmp eq i32 %17, 123, !dbg !38
  br i1 %18, label %assert.exit.L14, label %assert.then.L14, !dbg !38, !prof !30

assert.then.L14:                                  ; preds = %assert.exit.L13
  %19 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !38
  call void @exit(i32 1), !dbg !38
  unreachable, !dbg !38

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %20 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !39
  %21 = load i32, ptr %20, align 4, !dbg !40
  %22 = icmp eq i32 %21, 123, !dbg !40
  br i1 %22, label %assert.exit.L15, label %assert.then.L15, !dbg !40, !prof !30

assert.then.L15:                                  ; preds = %assert.exit.L14
  %23 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !40
  call void @exit(i32 1), !dbg !40
  unreachable, !dbg !40

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !41
  %24 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !42
  %25 = load i32, ptr %24, align 4, !dbg !43
  %26 = icmp eq i32 %25, 4321, !dbg !43
  br i1 %26, label %assert.exit.L17, label %assert.then.L17, !dbg !43, !prof !30

assert.then.L17:                                  ; preds = %assert.exit.L15
  %27 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !43
  call void @exit(i32 1), !dbg !43
  unreachable, !dbg !43

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %28 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !44
  br i1 %28, label %assert.exit.L18, label %assert.then.L18, !dbg !44, !prof !30

assert.then.L18:                                  ; preds = %assert.exit.L17
  %29 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !44
  call void @exit(i32 1), !dbg !44
  unreachable, !dbg !44

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !45
  %30 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr %it), !dbg !46
  store %__long_intref__Pair__long_intref %30, ptr %pair, align 8, !dbg !46
  call void @llvm.dbg.declare(metadata ptr %pair, metadata !47, metadata !DIExpression()), !dbg !46
  store %__long_intref__Pair__long_intref %30, ptr %pair, align 8, !dbg !46
  %31 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr %pair), !dbg !50
  %32 = load i64, ptr %31, align 8, !dbg !51
  %33 = icmp eq i64 %32, 2, !dbg !51
  br i1 %33, label %assert.exit.L21, label %assert.then.L21, !dbg !51, !prof !30

assert.then.L21:                                  ; preds = %assert.exit.L18
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !51
  call void @exit(i32 1), !dbg !51
  unreachable, !dbg !51

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %35 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr %pair), !dbg !52
  %36 = load i32, ptr %35, align 4, !dbg !53
  %37 = icmp eq i32 %36, 9876, !dbg !53
  br i1 %37, label %assert.exit.L22, label %assert.then.L22, !dbg !53, !prof !30

assert.then.L22:                                  ; preds = %assert.exit.L21
  %38 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !54
  %39 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !55
  %40 = xor i1 %39, true, !dbg !55
  store i1 %40, ptr %6, align 1, !dbg !55
  br i1 %40, label %assert.exit.L24, label %assert.then.L24, !dbg !55, !prof !30

assert.then.L24:                                  ; preds = %assert.exit.L22
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !55
  call void @exit(i32 1), !dbg !55
  unreachable, !dbg !55

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %7, align 4, !dbg !56
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %7), !dbg !56
  store i32 -99, ptr %8, align 4, !dbg !57
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %8), !dbg !57
  %42 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !58
  br i1 %42, label %assert.exit.L29, label %assert.then.L29, !dbg !58, !prof !30

assert.then.L29:                                  ; preds = %assert.exit.L24
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr %it, i32 3), !dbg !59
  %44 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !60
  %45 = load i32, ptr %44, align 4, !dbg !61
  %46 = icmp eq i32 %45, 123, !dbg !61
  br i1 %46, label %assert.exit.L33, label %assert.then.L33, !dbg !61, !prof !30

assert.then.L33:                                  ; preds = %assert.exit.L29
  %47 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !61
  call void @exit(i32 1), !dbg !61
  unreachable, !dbg !61

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %48 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !62
  br i1 %48, label %assert.exit.L34, label %assert.then.L34, !dbg !62, !prof !30

assert.then.L34:                                  ; preds = %assert.exit.L33
  %49 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !62
  call void @exit(i32 1), !dbg !62
  unreachable, !dbg !62

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %50 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !63
  call void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr %it), !dbg !63
  %51 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !64
  %52 = load i32, ptr %51, align 4, !dbg !65
  %53 = icmp eq i32 %52, 4321, !dbg !65
  br i1 %53, label %assert.exit.L36, label %assert.then.L36, !dbg !65, !prof !30

assert.then.L36:                                  ; preds = %assert.exit.L34
  %54 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !65
  call void @exit(i32 1), !dbg !65
  unreachable, !dbg !65

assert.exit.L36:                                  ; preds = %assert.exit.L34
  %55 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !66
  call void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr %it), !dbg !66
  %56 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !67
  %57 = load i32, ptr %56, align 4, !dbg !68
  %58 = icmp eq i32 %57, 123, !dbg !68
  br i1 %58, label %assert.exit.L38, label %assert.then.L38, !dbg !68, !prof !30

assert.then.L38:                                  ; preds = %assert.exit.L36
  %59 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !68
  call void @exit(i32 1), !dbg !68
  unreachable, !dbg !68

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr %it, i32 4), !dbg !69
  %60 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !70
  %61 = load i32, ptr %60, align 4, !dbg !71
  %62 = icmp eq i32 %61, -99, !dbg !71
  br i1 %62, label %assert.exit.L40, label %assert.then.L40, !dbg !71, !prof !30

assert.then.L40:                                  ; preds = %assert.exit.L38
  %63 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !71
  call void @exit(i32 1), !dbg !71
  unreachable, !dbg !71

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !72
  %64 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !73
  %65 = xor i1 %64, true, !dbg !73
  store i1 %65, ptr %9, align 1, !dbg !73
  br i1 %65, label %assert.exit.L42, label %assert.then.L42, !dbg !73, !prof !30

assert.then.L42:                                  ; preds = %assert.exit.L40
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !73
  call void @exit(i32 1), !dbg !73
  unreachable, !dbg !73

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %67 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !74
  %68 = load i32, ptr %result, align 4, !dbg !74
  ret i32 %68, !dbg !74
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
!6 = distinct !DISubprogram(name: "main", linkageName: "main", scope: !7, file: !7, line: 3, type: !8, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !4, retainedNodes: !13)
!7 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10, !11}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, elements: !13)
!12 = !DIBasicType(name: "string", size: 8, encoding: DW_ATE_ASCII)
!13 = !{}
!14 = !DILocalVariable(name: "result", scope: !6, file: !7, line: 3, type: !10)
!15 = !DILocation(line: 3, column: 1, scope: !6)
!16 = !DILocalVariable(name: "argc", arg: 1, scope: !6, file: !7, line: 3, type: !10)
!17 = !DILocalVariable(name: "argv", arg: 2, scope: !6, file: !7, line: 3, type: !11)
!18 = !DILocalVariable(name: "vi", scope: !6, file: !7, line: 5, type: !19)
!19 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !7, file: !7, line: 25, align: 8, flags: DIFlagTypePassByValue, elements: !20)
!20 = !{!21, !22, !22, !23}
!21 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!22 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!23 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!24 = !DILocation(line: 5, column: 22, scope: !6)
!25 = !DILocation(line: 6, column: 17, scope: !6)
!26 = !DILocation(line: 7, column: 17, scope: !6)
!27 = !DILocation(line: 8, column: 17, scope: !6)
!28 = !DILocation(line: 9, column: 12, scope: !6)
!29 = !DILocation(line: 9, column: 28, scope: !6)
!30 = !{!"branch_weights", i32 2000, i32 1}
!31 = !DILocation(line: 12, column: 22, scope: !6)
!32 = !DILocalVariable(name: "it", scope: !6, file: !7, line: 12, type: !33)
!33 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !7, file: !7, line: 11, align: 8, flags: DIFlagTypePassByValue, elements: !34)
!34 = !{!35, !22}
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64)
!36 = !DILocation(line: 13, column: 12, scope: !6)
!37 = !DILocation(line: 14, column: 12, scope: !6)
!38 = !DILocation(line: 14, column: 24, scope: !6)
!39 = !DILocation(line: 15, column: 12, scope: !6)
!40 = !DILocation(line: 15, column: 24, scope: !6)
!41 = !DILocation(line: 16, column: 5, scope: !6)
!42 = !DILocation(line: 17, column: 12, scope: !6)
!43 = !DILocation(line: 17, column: 24, scope: !6)
!44 = !DILocation(line: 18, column: 12, scope: !6)
!45 = !DILocation(line: 19, column: 5, scope: !6)
!46 = !DILocation(line: 20, column: 16, scope: !6)
!47 = !DILocalVariable(name: "pair", scope: !6, file: !7, line: 20, type: !48)
!48 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !7, file: !7, line: 8, align: 8, flags: DIFlagTypePassByValue, elements: !49)
!49 = !{!22, !21}
!50 = !DILocation(line: 21, column: 12, scope: !6)
!51 = !DILocation(line: 21, column: 31, scope: !6)
!52 = !DILocation(line: 22, column: 12, scope: !6)
!53 = !DILocation(line: 22, column: 32, scope: !6)
!54 = !DILocation(line: 23, column: 5, scope: !6)
!55 = !DILocation(line: 24, column: 13, scope: !6)
!56 = !DILocation(line: 27, column: 17, scope: !6)
!57 = !DILocation(line: 28, column: 17, scope: !6)
!58 = !DILocation(line: 29, column: 12, scope: !6)
!59 = !DILocation(line: 32, column: 5, scope: !6)
!60 = !DILocation(line: 33, column: 12, scope: !6)
!61 = !DILocation(line: 33, column: 24, scope: !6)
!62 = !DILocation(line: 34, column: 12, scope: !6)
!63 = !DILocation(line: 35, column: 5, scope: !6)
!64 = !DILocation(line: 36, column: 12, scope: !6)
!65 = !DILocation(line: 36, column: 24, scope: !6)
!66 = !DILocation(line: 37, column: 5, scope: !6)
!67 = !DILocation(line: 38, column: 12, scope: !6)
!68 = !DILocation(line: 38, column: 24, scope: !6)
!69 = !DILocation(line: 39, column: 5, scope: !6)
!70 = !DILocation(line: 40, column: 12, scope: !6)
!71 = !DILocation(line: 40, column: 24, scope: !6)
!72 = !DILocation(line: 41, column: 5, scope: !6)
!73 = !DILocation(line: 42, column: 13, scope: !6)
!74 = !DILocation(line: 44, column: 5, scope: !6)
