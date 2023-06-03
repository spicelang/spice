; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Vector__intptr_long_long = type { ptr, i64, i64 }
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
@anon.string.16 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 123' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4321' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9876' evaluated to false.\00", align 1
@anon.string.19 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4322' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9877' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4323' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9879' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1, !dbg !0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 !dbg !15 {
  %result = alloca i32, align 4
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
  %vi = alloca %__int__Vector__intptr_long_long, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %it = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %pair = alloca %__long_intref__Pair__long_intref, align 8
  %6 = alloca i1, align 1
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i1, align 1
  %10 = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %item = alloca i32, align 4
  %11 = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %item1 = alloca ptr, align 8
  %12 = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %idx = alloca i64, align 8
  %item2 = alloca ptr, align 8
  %pair_addr = alloca %__long_intref__Pair__long_intref, align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !23, metadata !DIExpression()), !dbg !24
  store i32 0, ptr %result, align 4, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %argc, metadata !25, metadata !DIExpression()), !dbg !24
  store i32 %0, ptr %argc, align 4, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %argv, metadata !26, metadata !DIExpression()), !dbg !24
  store ptr %1, ptr %argv, align 8, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %vi, metadata !27, metadata !DIExpression()), !dbg !35
  call void @_mp__Vector_int__void__ctor(ptr %vi), !dbg !35
  store i32 123, ptr %3, align 4, !dbg !36
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %3), !dbg !36
  store i32 4321, ptr %4, align 4, !dbg !37
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %4), !dbg !37
  store i32 9876, ptr %5, align 4, !dbg !38
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %5), !dbg !38
  %13 = call i64 @_mf__Vector_int__long__getSize(ptr %vi), !dbg !39
  %14 = icmp eq i64 %13, 3, !dbg !40
  br i1 %14, label %assert.exit.L9, label %assert.then.L9, !dbg !40, !prof !41

assert.then.L9:                                   ; preds = %2
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !40
  call void @exit(i32 1), !dbg !40
  unreachable, !dbg !40

assert.exit.L9:                                   ; preds = %2
  %16 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !42
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %16, ptr %it, align 8, !dbg !42
  call void @llvm.dbg.declare(metadata ptr %it, metadata !43, metadata !DIExpression()), !dbg !42
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %16, ptr %it, align 8, !dbg !42
  %17 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !54
  br i1 %17, label %assert.exit.L13, label %assert.then.L13, !dbg !54, !prof !41

assert.then.L13:                                  ; preds = %assert.exit.L9
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !54
  call void @exit(i32 1), !dbg !54
  unreachable, !dbg !54

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %19 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !55
  %20 = load i32, ptr %19, align 4, !dbg !56
  %21 = icmp eq i32 %20, 123, !dbg !56
  br i1 %21, label %assert.exit.L14, label %assert.then.L14, !dbg !56, !prof !41

assert.then.L14:                                  ; preds = %assert.exit.L13
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !56
  call void @exit(i32 1), !dbg !56
  unreachable, !dbg !56

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %23 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !57
  %24 = load i32, ptr %23, align 4, !dbg !58
  %25 = icmp eq i32 %24, 123, !dbg !58
  br i1 %25, label %assert.exit.L15, label %assert.then.L15, !dbg !58, !prof !41

assert.then.L15:                                  ; preds = %assert.exit.L14
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !59
  %27 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !60
  %28 = load i32, ptr %27, align 4, !dbg !61
  %29 = icmp eq i32 %28, 4321, !dbg !61
  br i1 %29, label %assert.exit.L17, label %assert.then.L17, !dbg !61, !prof !41

assert.then.L17:                                  ; preds = %assert.exit.L15
  %30 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !61
  call void @exit(i32 1), !dbg !61
  unreachable, !dbg !61

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %31 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !62
  br i1 %31, label %assert.exit.L18, label %assert.then.L18, !dbg !62, !prof !41

assert.then.L18:                                  ; preds = %assert.exit.L17
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !62
  call void @exit(i32 1), !dbg !62
  unreachable, !dbg !62

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !63
  %33 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr %it), !dbg !64
  store %__long_intref__Pair__long_intref %33, ptr %pair, align 8, !dbg !64
  call void @llvm.dbg.declare(metadata ptr %pair, metadata !65, metadata !DIExpression()), !dbg !64
  store %__long_intref__Pair__long_intref %33, ptr %pair, align 8, !dbg !64
  %34 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr %pair), !dbg !70
  %35 = load i64, ptr %34, align 8, !dbg !71
  %36 = icmp eq i64 %35, 2, !dbg !71
  br i1 %36, label %assert.exit.L21, label %assert.then.L21, !dbg !71, !prof !41

assert.then.L21:                                  ; preds = %assert.exit.L18
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !71
  call void @exit(i32 1), !dbg !71
  unreachable, !dbg !71

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %38 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr %pair), !dbg !72
  %39 = load i32, ptr %38, align 4, !dbg !73
  %40 = icmp eq i32 %39, 9876, !dbg !73
  br i1 %40, label %assert.exit.L22, label %assert.then.L22, !dbg !73, !prof !41

assert.then.L22:                                  ; preds = %assert.exit.L21
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !73
  call void @exit(i32 1), !dbg !73
  unreachable, !dbg !73

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !74
  %42 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !75
  %43 = xor i1 %42, true, !dbg !75
  store i1 %43, ptr %6, align 1, !dbg !75
  br i1 %43, label %assert.exit.L24, label %assert.then.L24, !dbg !75, !prof !41

assert.then.L24:                                  ; preds = %assert.exit.L22
  %44 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !75
  call void @exit(i32 1), !dbg !75
  unreachable, !dbg !75

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %7, align 4, !dbg !76
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %7), !dbg !76
  store i32 -99, ptr %8, align 4, !dbg !77
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %8), !dbg !77
  %45 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !78
  br i1 %45, label %assert.exit.L29, label %assert.then.L29, !dbg !78, !prof !41

assert.then.L29:                                  ; preds = %assert.exit.L24
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !78
  call void @exit(i32 1), !dbg !78
  unreachable, !dbg !78

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr %it, i32 3), !dbg !79
  %47 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !80
  %48 = load i32, ptr %47, align 4, !dbg !81
  %49 = icmp eq i32 %48, 123, !dbg !81
  br i1 %49, label %assert.exit.L33, label %assert.then.L33, !dbg !81, !prof !41

assert.then.L33:                                  ; preds = %assert.exit.L29
  %50 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !81
  call void @exit(i32 1), !dbg !81
  unreachable, !dbg !81

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %51 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !82
  br i1 %51, label %assert.exit.L34, label %assert.then.L34, !dbg !82, !prof !41

assert.then.L34:                                  ; preds = %assert.exit.L33
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !82
  call void @exit(i32 1), !dbg !82
  unreachable, !dbg !82

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %53 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !83
  call void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr %it), !dbg !83
  %54 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !84
  %55 = load i32, ptr %54, align 4, !dbg !85
  %56 = icmp eq i32 %55, 4321, !dbg !85
  br i1 %56, label %assert.exit.L36, label %assert.then.L36, !dbg !85, !prof !41

assert.then.L36:                                  ; preds = %assert.exit.L34
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !85
  call void @exit(i32 1), !dbg !85
  unreachable, !dbg !85

assert.exit.L36:                                  ; preds = %assert.exit.L34
  %58 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !86
  call void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr %it), !dbg !86
  %59 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !87
  %60 = load i32, ptr %59, align 4, !dbg !88
  %61 = icmp eq i32 %60, 123, !dbg !88
  br i1 %61, label %assert.exit.L38, label %assert.then.L38, !dbg !88, !prof !41

assert.then.L38:                                  ; preds = %assert.exit.L36
  %62 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !88
  call void @exit(i32 1), !dbg !88
  unreachable, !dbg !88

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr %it, i32 4), !dbg !89
  %63 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !90
  %64 = load i32, ptr %63, align 4, !dbg !91
  %65 = icmp eq i32 %64, -99, !dbg !91
  br i1 %65, label %assert.exit.L40, label %assert.then.L40, !dbg !91, !prof !41

assert.then.L40:                                  ; preds = %assert.exit.L38
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !91
  call void @exit(i32 1), !dbg !91
  unreachable, !dbg !91

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !92
  %67 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !93
  %68 = xor i1 %67, true, !dbg !93
  store i1 %68, ptr %9, align 1, !dbg !93
  br i1 %68, label %assert.exit.L42, label %assert.then.L42, !dbg !93, !prof !41

assert.then.L42:                                  ; preds = %assert.exit.L40
  %69 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !93
  call void @exit(i32 1), !dbg !93
  unreachable, !dbg !93

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %70 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !94
  call void @llvm.dbg.declare(metadata ptr %item, metadata !95, metadata !DIExpression()), !dbg !96
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %70, ptr %10, align 8, !dbg !94
  br label %foreach.head.L45, !dbg !96

foreach.head.L45:                                 ; preds = %foreach.tail.L45, %assert.exit.L42
  %71 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %10), !dbg !96
  br i1 %71, label %foreach.body.L45, label %foreach.exit.L45, !dbg !96

foreach.body.L45:                                 ; preds = %foreach.head.L45
  %72 = call ptr @_mf__VectorIterator_int__intref__get(ptr %10), !dbg !96
  %73 = load i32, ptr %72, align 4, !dbg !96
  store i32 %73, ptr %item, align 4, !dbg !96
  %74 = load i32, ptr %item, align 4, !dbg !97
  %75 = add i32 %74, 1, !dbg !97
  store i32 %75, ptr %item, align 4, !dbg !97
  br label %foreach.tail.L45, !dbg !97

foreach.tail.L45:                                 ; preds = %foreach.body.L45
  call void @_mp__VectorIterator_int__void__next(ptr %10), !dbg !97
  br label %foreach.head.L45, !dbg !97

foreach.exit.L45:                                 ; preds = %foreach.head.L45
  %76 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 0), !dbg !98
  %77 = load i32, ptr %76, align 4, !dbg !99
  %78 = icmp eq i32 %77, 123, !dbg !99
  br i1 %78, label %assert.exit.L48, label %assert.then.L48, !dbg !99, !prof !41

assert.then.L48:                                  ; preds = %foreach.exit.L45
  %79 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !99
  call void @exit(i32 1), !dbg !99
  unreachable, !dbg !99

assert.exit.L48:                                  ; preds = %foreach.exit.L45
  %80 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 1), !dbg !100
  %81 = load i32, ptr %80, align 4, !dbg !101
  %82 = icmp eq i32 %81, 4321, !dbg !101
  br i1 %82, label %assert.exit.L49, label %assert.then.L49, !dbg !101, !prof !41

assert.then.L49:                                  ; preds = %assert.exit.L48
  %83 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !101
  call void @exit(i32 1), !dbg !101
  unreachable, !dbg !101

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %84 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 2), !dbg !102
  %85 = load i32, ptr %84, align 4, !dbg !103
  %86 = icmp eq i32 %85, 9876, !dbg !103
  br i1 %86, label %assert.exit.L50, label %assert.then.L50, !dbg !103, !prof !41

assert.then.L50:                                  ; preds = %assert.exit.L49
  %87 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !103
  call void @exit(i32 1), !dbg !103
  unreachable, !dbg !103

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %88 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !104
  call void @llvm.dbg.declare(metadata ptr %item1, metadata !105, metadata !DIExpression()), !dbg !106
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %88, ptr %11, align 8, !dbg !104
  br label %foreach.head.L53, !dbg !106

foreach.head.L53:                                 ; preds = %foreach.tail.L53, %assert.exit.L50
  %89 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %11), !dbg !106
  br i1 %89, label %foreach.body.L53, label %foreach.exit.L53, !dbg !106

foreach.body.L53:                                 ; preds = %foreach.head.L53
  %90 = call ptr @_mf__VectorIterator_int__intref__get(ptr %11), !dbg !106
  store ptr %90, ptr %item1, align 8, !dbg !106
  %91 = load ptr, ptr %item1, align 8, !dbg !107
  %92 = load i32, ptr %91, align 4, !dbg !107
  %93 = add i32 %92, 1, !dbg !107
  store i32 %93, ptr %91, align 4, !dbg !107
  br label %foreach.tail.L53, !dbg !107

foreach.tail.L53:                                 ; preds = %foreach.body.L53
  call void @_mp__VectorIterator_int__void__next(ptr %11), !dbg !107
  br label %foreach.head.L53, !dbg !107

foreach.exit.L53:                                 ; preds = %foreach.head.L53
  %94 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 0), !dbg !108
  %95 = load i32, ptr %94, align 4, !dbg !109
  %96 = icmp eq i32 %95, 124, !dbg !109
  br i1 %96, label %assert.exit.L56, label %assert.then.L56, !dbg !109, !prof !41

assert.then.L56:                                  ; preds = %foreach.exit.L53
  %97 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !109
  call void @exit(i32 1), !dbg !109
  unreachable, !dbg !109

assert.exit.L56:                                  ; preds = %foreach.exit.L53
  %98 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 1), !dbg !110
  %99 = load i32, ptr %98, align 4, !dbg !111
  %100 = icmp eq i32 %99, 4322, !dbg !111
  br i1 %100, label %assert.exit.L57, label %assert.then.L57, !dbg !111, !prof !41

assert.then.L57:                                  ; preds = %assert.exit.L56
  %101 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !111
  call void @exit(i32 1), !dbg !111
  unreachable, !dbg !111

assert.exit.L57:                                  ; preds = %assert.exit.L56
  %102 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 2), !dbg !112
  %103 = load i32, ptr %102, align 4, !dbg !113
  %104 = icmp eq i32 %103, 9877, !dbg !113
  br i1 %104, label %assert.exit.L58, label %assert.then.L58, !dbg !113, !prof !41

assert.then.L58:                                  ; preds = %assert.exit.L57
  %105 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !113
  call void @exit(i32 1), !dbg !113
  unreachable, !dbg !113

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %106 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !114
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %106, ptr %12, align 8, !dbg !114
  call void @llvm.dbg.declare(metadata ptr %idx, metadata !115, metadata !DIExpression()), !dbg !117
  call void @llvm.dbg.declare(metadata ptr %item2, metadata !118, metadata !DIExpression()), !dbg !119
  store i64 0, ptr %idx, align 8, !dbg !117
  br label %foreach.head.L60, !dbg !119

foreach.head.L60:                                 ; preds = %foreach.tail.L60, %assert.exit.L58
  %107 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %12), !dbg !119
  br i1 %107, label %foreach.body.L60, label %foreach.exit.L60, !dbg !119

foreach.body.L60:                                 ; preds = %foreach.head.L60
  %pair3 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr %12), !dbg !119
  store %__long_intref__Pair__long_intref %pair3, ptr %pair_addr, align 8, !dbg !119
  %idx_addr = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %pair_addr, i32 0, i32 0, !dbg !119
  %108 = load i64, ptr %idx_addr, align 8, !dbg !119
  store i64 %108, ptr %idx, align 8, !dbg !119
  %item_addr = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %pair_addr, i32 0, i32 1, !dbg !119
  %109 = load ptr, ptr %item_addr, align 8, !dbg !119
  store ptr %109, ptr %item2, align 8, !dbg !119
  %110 = load i64, ptr %idx, align 8, !dbg !120
  %111 = trunc i64 %110 to i32, !dbg !120
  %112 = load ptr, ptr %item2, align 8, !dbg !120
  %113 = load i32, ptr %112, align 4, !dbg !120
  %114 = add i32 %113, %111, !dbg !120
  store i32 %114, ptr %112, align 4, !dbg !120
  br label %foreach.tail.L60, !dbg !120

foreach.tail.L60:                                 ; preds = %foreach.body.L60
  call void @_mp__VectorIterator_int__void__next(ptr %12), !dbg !120
  br label %foreach.head.L60, !dbg !120

foreach.exit.L60:                                 ; preds = %foreach.head.L60
  %115 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 0), !dbg !121
  %116 = load i32, ptr %115, align 4, !dbg !122
  %117 = icmp eq i32 %116, 124, !dbg !122
  br i1 %117, label %assert.exit.L63, label %assert.then.L63, !dbg !122, !prof !41

assert.then.L63:                                  ; preds = %foreach.exit.L60
  %118 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !122
  call void @exit(i32 1), !dbg !122
  unreachable, !dbg !122

assert.exit.L63:                                  ; preds = %foreach.exit.L60
  %119 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 1), !dbg !123
  %120 = load i32, ptr %119, align 4, !dbg !124
  %121 = icmp eq i32 %120, 4323, !dbg !124
  br i1 %121, label %assert.exit.L64, label %assert.then.L64, !dbg !124, !prof !41

assert.then.L64:                                  ; preds = %assert.exit.L63
  %122 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !124
  call void @exit(i32 1), !dbg !124
  unreachable, !dbg !124

assert.exit.L64:                                  ; preds = %assert.exit.L63
  %123 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 2), !dbg !125
  %124 = load i32, ptr %123, align 4, !dbg !126
  %125 = icmp eq i32 %124, 9879, !dbg !126
  br i1 %125, label %assert.exit.L65, label %assert.then.L65, !dbg !126, !prof !41

assert.then.L65:                                  ; preds = %assert.exit.L64
  %126 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !126
  call void @exit(i32 1), !dbg !126
  unreachable, !dbg !126

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %127 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !127
  %128 = load i32, ptr %result, align 4, !dbg !127
  ret i32 %128, !dbg !127
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

declare ptr @_mf__Vector_int__intref__get__int(ptr, i32)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.module.flags = !{!7, !8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !5, line: 67, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C11, file: !3, producer: "spice version dev", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "C:\\Users\\Marc\\Documents\\JustForFunGitHubClonesFast\\spice\\cmake-build-debug\\test\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex\\source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!6 = !DIStringType(name: "printf.str.0", size: 184)
!7 = !{i32 7, !"Dwarf Version", i32 4}
!8 = !{i32 2, !"Debug Info Version", i32 3}
!9 = !{i32 1, !"wchar_size", i32 4}
!10 = !{i32 8, !"PIC Level", i32 2}
!11 = !{i32 7, !"PIE Level", i32 0}
!12 = !{i32 7, !"uwtable", i32 2}
!13 = !{i32 7, !"frame-pointer", i32 2}
!14 = !{!"spice version dev"}
!15 = distinct !DISubprogram(name: "main", linkageName: "main", scope: !5, file: !5, line: 3, type: !16, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !22)
!16 = !DISubroutineType(types: !17)
!17 = !{!18, !18, !19}
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !20, elements: !22)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !21, size: 64)
!21 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!22 = !{}
!23 = !DILocalVariable(name: "result", scope: !15, file: !5, line: 3, type: !18)
!24 = !DILocation(line: 3, column: 1, scope: !15)
!25 = !DILocalVariable(name: "argc", arg: 1, scope: !15, file: !5, line: 3, type: !18)
!26 = !DILocalVariable(name: "argv", arg: 2, scope: !15, file: !5, line: 3, type: !19)
!27 = !DILocalVariable(name: "vi", scope: !15, file: !5, line: 5, type: !28)
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !5, file: !5, line: 25, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !29, identifier: "__int__Vector__intptr_long_long")
!29 = !{!30, !32, !34}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !28, file: !5, line: 26, baseType: !31, size: 64)
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !28, file: !5, line: 27, baseType: !33, size: 64, offset: 64)
!33 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !28, file: !5, line: 28, baseType: !33, size: 64, offset: 128)
!35 = !DILocation(line: 5, column: 22, scope: !15)
!36 = !DILocation(line: 6, column: 17, scope: !15)
!37 = !DILocation(line: 7, column: 17, scope: !15)
!38 = !DILocation(line: 8, column: 17, scope: !15)
!39 = !DILocation(line: 9, column: 12, scope: !15)
!40 = !DILocation(line: 9, column: 28, scope: !15)
!41 = !{!"branch_weights", i32 2000, i32 1}
!42 = !DILocation(line: 12, column: 22, scope: !15)
!43 = !DILocalVariable(name: "it", scope: !15, file: !5, line: 12, type: !44)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !5, file: !5, line: 11, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !45, identifier: "__int__VectorIterator__std/data/vector::Vector<int>ref_long")
!45 = !{!46, !53}
!46 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !44, file: !5, line: 12, baseType: !47, size: 64)
!47 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !48, size: 64)
!48 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !5, file: !5, line: 25, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !49, identifier: "__int__Vector__intptr_long_long")
!49 = !{!50, !51, !52}
!50 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !48, file: !5, line: 26, baseType: !31, size: 64)
!51 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !48, file: !5, line: 27, baseType: !33, size: 64, offset: 64)
!52 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !48, file: !5, line: 28, baseType: !33, size: 64, offset: 128)
!53 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !44, file: !5, line: 13, baseType: !33, size: 64, offset: 64)
!54 = !DILocation(line: 13, column: 12, scope: !15)
!55 = !DILocation(line: 14, column: 12, scope: !15)
!56 = !DILocation(line: 14, column: 24, scope: !15)
!57 = !DILocation(line: 15, column: 12, scope: !15)
!58 = !DILocation(line: 15, column: 24, scope: !15)
!59 = !DILocation(line: 16, column: 5, scope: !15)
!60 = !DILocation(line: 17, column: 12, scope: !15)
!61 = !DILocation(line: 17, column: 24, scope: !15)
!62 = !DILocation(line: 18, column: 12, scope: !15)
!63 = !DILocation(line: 19, column: 5, scope: !15)
!64 = !DILocation(line: 20, column: 16, scope: !15)
!65 = !DILocalVariable(name: "pair", scope: !15, file: !5, line: 20, type: !66)
!66 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !5, file: !5, line: 8, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !67, identifier: "__long_intref__Pair__long_intref")
!67 = !{!68, !69}
!68 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !66, file: !5, line: 9, baseType: !33, size: 64)
!69 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !66, file: !5, line: 10, baseType: !31, size: 64, offset: 64)
!70 = !DILocation(line: 21, column: 12, scope: !15)
!71 = !DILocation(line: 21, column: 31, scope: !15)
!72 = !DILocation(line: 22, column: 12, scope: !15)
!73 = !DILocation(line: 22, column: 32, scope: !15)
!74 = !DILocation(line: 23, column: 5, scope: !15)
!75 = !DILocation(line: 24, column: 13, scope: !15)
!76 = !DILocation(line: 27, column: 17, scope: !15)
!77 = !DILocation(line: 28, column: 17, scope: !15)
!78 = !DILocation(line: 29, column: 12, scope: !15)
!79 = !DILocation(line: 32, column: 5, scope: !15)
!80 = !DILocation(line: 33, column: 12, scope: !15)
!81 = !DILocation(line: 33, column: 24, scope: !15)
!82 = !DILocation(line: 34, column: 12, scope: !15)
!83 = !DILocation(line: 35, column: 5, scope: !15)
!84 = !DILocation(line: 36, column: 12, scope: !15)
!85 = !DILocation(line: 36, column: 24, scope: !15)
!86 = !DILocation(line: 37, column: 5, scope: !15)
!87 = !DILocation(line: 38, column: 12, scope: !15)
!88 = !DILocation(line: 38, column: 24, scope: !15)
!89 = !DILocation(line: 39, column: 5, scope: !15)
!90 = !DILocation(line: 40, column: 12, scope: !15)
!91 = !DILocation(line: 40, column: 24, scope: !15)
!92 = !DILocation(line: 41, column: 5, scope: !15)
!93 = !DILocation(line: 42, column: 13, scope: !15)
!94 = !DILocation(line: 45, column: 32, scope: !15)
!95 = !DILocalVariable(name: "item", scope: !15, file: !5, line: 45, type: !18)
!96 = !DILocation(line: 45, column: 13, scope: !15)
!97 = !DILocation(line: 46, column: 9, scope: !15)
!98 = !DILocation(line: 48, column: 19, scope: !15)
!99 = !DILocation(line: 48, column: 25, scope: !15)
!100 = !DILocation(line: 49, column: 19, scope: !15)
!101 = !DILocation(line: 49, column: 25, scope: !15)
!102 = !DILocation(line: 50, column: 19, scope: !15)
!103 = !DILocation(line: 50, column: 25, scope: !15)
!104 = !DILocation(line: 53, column: 33, scope: !15)
!105 = !DILocalVariable(name: "item", scope: !15, file: !5, line: 53, type: !31)
!106 = !DILocation(line: 53, column: 13, scope: !15)
!107 = !DILocation(line: 54, column: 9, scope: !15)
!108 = !DILocation(line: 56, column: 19, scope: !15)
!109 = !DILocation(line: 56, column: 25, scope: !15)
!110 = !DILocation(line: 57, column: 19, scope: !15)
!111 = !DILocation(line: 57, column: 25, scope: !15)
!112 = !DILocation(line: 58, column: 19, scope: !15)
!113 = !DILocation(line: 58, column: 25, scope: !15)
!114 = !DILocation(line: 60, column: 43, scope: !15)
!115 = !DILocalVariable(name: "idx", scope: !15, file: !5, line: 60, type: !116)
!116 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!117 = !DILocation(line: 60, column: 13, scope: !15)
!118 = !DILocalVariable(name: "item", scope: !15, file: !5, line: 60, type: !31)
!119 = !DILocation(line: 60, column: 23, scope: !15)
!120 = !DILocation(line: 61, column: 9, scope: !15)
!121 = !DILocation(line: 63, column: 19, scope: !15)
!122 = !DILocation(line: 63, column: 25, scope: !15)
!123 = !DILocation(line: 64, column: 19, scope: !15)
!124 = !DILocation(line: 64, column: 25, scope: !15)
!125 = !DILocation(line: 65, column: 19, scope: !15)
!126 = !DILocation(line: 65, column: 25, scope: !15)
!127 = !DILocation(line: 67, column: 5, scope: !15)
