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
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 !dbg !6 {
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
  call void @llvm.dbg.declare(metadata ptr %result, metadata !15, metadata !DIExpression()), !dbg !16
  store i32 0, ptr %result, align 4, !dbg !16
  call void @llvm.dbg.declare(metadata ptr %argc, metadata !17, metadata !DIExpression()), !dbg !16
  store i32 %0, ptr %argc, align 4, !dbg !16
  call void @llvm.dbg.declare(metadata ptr %argv, metadata !18, metadata !DIExpression()), !dbg !16
  store ptr %1, ptr %argv, align 8, !dbg !16
  call void @llvm.dbg.declare(metadata ptr %vi, metadata !19, metadata !DIExpression()), !dbg !24
  call void @_mp__Vector_int__void__ctor(ptr %vi), !dbg !24
  store i32 123, ptr %3, align 4, !dbg !25
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %3), !dbg !25
  store i32 4321, ptr %4, align 4, !dbg !26
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %4), !dbg !26
  store i32 9876, ptr %5, align 4, !dbg !27
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %5), !dbg !27
  %13 = call i64 @_mf__Vector_int__long__getSize(ptr %vi), !dbg !28
  %14 = icmp eq i64 %13, 3, !dbg !29
  br i1 %14, label %assert.exit.L9, label %assert.then.L9, !dbg !29, !prof !30

assert.then.L9:                                   ; preds = %2
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !29
  call void @exit(i32 1), !dbg !29
  unreachable, !dbg !29

assert.exit.L9:                                   ; preds = %2
  %16 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !31
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %16, ptr %it, align 8, !dbg !31
  call void @llvm.dbg.declare(metadata ptr %it, metadata !32, metadata !DIExpression()), !dbg !31
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %16, ptr %it, align 8, !dbg !31
  %17 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !36
  br i1 %17, label %assert.exit.L13, label %assert.then.L13, !dbg !36, !prof !30

assert.then.L13:                                  ; preds = %assert.exit.L9
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !36
  call void @exit(i32 1), !dbg !36
  unreachable, !dbg !36

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %19 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !37
  %20 = load i32, ptr %19, align 4, !dbg !38
  %21 = icmp eq i32 %20, 123, !dbg !38
  br i1 %21, label %assert.exit.L14, label %assert.then.L14, !dbg !38, !prof !30

assert.then.L14:                                  ; preds = %assert.exit.L13
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !38
  call void @exit(i32 1), !dbg !38
  unreachable, !dbg !38

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %23 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !39
  %24 = load i32, ptr %23, align 4, !dbg !40
  %25 = icmp eq i32 %24, 123, !dbg !40
  br i1 %25, label %assert.exit.L15, label %assert.then.L15, !dbg !40, !prof !30

assert.then.L15:                                  ; preds = %assert.exit.L14
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !40
  call void @exit(i32 1), !dbg !40
  unreachable, !dbg !40

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !41
  %27 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !42
  %28 = load i32, ptr %27, align 4, !dbg !43
  %29 = icmp eq i32 %28, 4321, !dbg !43
  br i1 %29, label %assert.exit.L17, label %assert.then.L17, !dbg !43, !prof !30

assert.then.L17:                                  ; preds = %assert.exit.L15
  %30 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !43
  call void @exit(i32 1), !dbg !43
  unreachable, !dbg !43

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %31 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !44
  br i1 %31, label %assert.exit.L18, label %assert.then.L18, !dbg !44, !prof !30

assert.then.L18:                                  ; preds = %assert.exit.L17
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !44
  call void @exit(i32 1), !dbg !44
  unreachable, !dbg !44

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !45
  %33 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr %it), !dbg !46
  store %__long_intref__Pair__long_intref %33, ptr %pair, align 8, !dbg !46
  call void @llvm.dbg.declare(metadata ptr %pair, metadata !47, metadata !DIExpression()), !dbg !46
  store %__long_intref__Pair__long_intref %33, ptr %pair, align 8, !dbg !46
  %34 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr %pair), !dbg !50
  %35 = load i64, ptr %34, align 8, !dbg !51
  %36 = icmp eq i64 %35, 2, !dbg !51
  br i1 %36, label %assert.exit.L21, label %assert.then.L21, !dbg !51, !prof !30

assert.then.L21:                                  ; preds = %assert.exit.L18
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !51
  call void @exit(i32 1), !dbg !51
  unreachable, !dbg !51

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %38 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr %pair), !dbg !52
  %39 = load i32, ptr %38, align 4, !dbg !53
  %40 = icmp eq i32 %39, 9876, !dbg !53
  br i1 %40, label %assert.exit.L22, label %assert.then.L22, !dbg !53, !prof !30

assert.then.L22:                                  ; preds = %assert.exit.L21
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !54
  %42 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !55
  %43 = xor i1 %42, true, !dbg !55
  store i1 %43, ptr %6, align 1, !dbg !55
  br i1 %43, label %assert.exit.L24, label %assert.then.L24, !dbg !55, !prof !30

assert.then.L24:                                  ; preds = %assert.exit.L22
  %44 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !55
  call void @exit(i32 1), !dbg !55
  unreachable, !dbg !55

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %7, align 4, !dbg !56
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %7), !dbg !56
  store i32 -99, ptr %8, align 4, !dbg !57
  call void @_mp__Vector_int__void__pushBack__intref(ptr %vi, ptr %8), !dbg !57
  %45 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !58
  br i1 %45, label %assert.exit.L29, label %assert.then.L29, !dbg !58, !prof !30

assert.then.L29:                                  ; preds = %assert.exit.L24
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr %it, i32 3), !dbg !59
  %47 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !60
  %48 = load i32, ptr %47, align 4, !dbg !61
  %49 = icmp eq i32 %48, 123, !dbg !61
  br i1 %49, label %assert.exit.L33, label %assert.then.L33, !dbg !61, !prof !30

assert.then.L33:                                  ; preds = %assert.exit.L29
  %50 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !61
  call void @exit(i32 1), !dbg !61
  unreachable, !dbg !61

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %51 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !62
  br i1 %51, label %assert.exit.L34, label %assert.then.L34, !dbg !62, !prof !30

assert.then.L34:                                  ; preds = %assert.exit.L33
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !62
  call void @exit(i32 1), !dbg !62
  unreachable, !dbg !62

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %53 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !63
  call void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr %it), !dbg !63
  %54 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !64
  %55 = load i32, ptr %54, align 4, !dbg !65
  %56 = icmp eq i32 %55, 4321, !dbg !65
  br i1 %56, label %assert.exit.L36, label %assert.then.L36, !dbg !65, !prof !30

assert.then.L36:                                  ; preds = %assert.exit.L34
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !65
  call void @exit(i32 1), !dbg !65
  unreachable, !dbg !65

assert.exit.L36:                                  ; preds = %assert.exit.L34
  %58 = load %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, align 8, !dbg !66
  call void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr %it), !dbg !66
  %59 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !67
  %60 = load i32, ptr %59, align 4, !dbg !68
  %61 = icmp eq i32 %60, 123, !dbg !68
  br i1 %61, label %assert.exit.L38, label %assert.then.L38, !dbg !68, !prof !30

assert.then.L38:                                  ; preds = %assert.exit.L36
  %62 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !68
  call void @exit(i32 1), !dbg !68
  unreachable, !dbg !68

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr %it, i32 4), !dbg !69
  %63 = call ptr @_mf__VectorIterator_int__intref__get(ptr %it), !dbg !70
  %64 = load i32, ptr %63, align 4, !dbg !71
  %65 = icmp eq i32 %64, -99, !dbg !71
  br i1 %65, label %assert.exit.L40, label %assert.then.L40, !dbg !71, !prof !30

assert.then.L40:                                  ; preds = %assert.exit.L38
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !71
  call void @exit(i32 1), !dbg !71
  unreachable, !dbg !71

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_mp__VectorIterator_int__void__next(ptr %it), !dbg !72
  %67 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %it), !dbg !73
  %68 = xor i1 %67, true, !dbg !73
  store i1 %68, ptr %9, align 1, !dbg !73
  br i1 %68, label %assert.exit.L42, label %assert.then.L42, !dbg !73, !prof !30

assert.then.L42:                                  ; preds = %assert.exit.L40
  %69 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !73
  call void @exit(i32 1), !dbg !73
  unreachable, !dbg !73

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %70 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !74
  call void @llvm.dbg.declare(metadata ptr %item, metadata !75, metadata !DIExpression()), !dbg !76
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %70, ptr %10, align 8, !dbg !74
  br label %foreach.head.L45, !dbg !76

foreach.head.L45:                                 ; preds = %foreach.tail.L45, %assert.exit.L42
  %71 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %10), !dbg !76
  br i1 %71, label %foreach.body.L45, label %foreach.exit.L45, !dbg !76

foreach.body.L45:                                 ; preds = %foreach.head.L45
  %72 = call ptr @_mf__VectorIterator_int__intref__get(ptr %10), !dbg !76
  %73 = load i32, ptr %72, align 4, !dbg !76
  store i32 %73, ptr %item, align 4, !dbg !76
  %74 = load i32, ptr %item, align 4, !dbg !77
  %75 = add i32 %74, 1, !dbg !77
  store i32 %75, ptr %item, align 4, !dbg !77
  br label %foreach.tail.L45, !dbg !77

foreach.tail.L45:                                 ; preds = %foreach.body.L45
  call void @_mp__VectorIterator_int__void__next(ptr %10), !dbg !77
  br label %foreach.head.L45, !dbg !77

foreach.exit.L45:                                 ; preds = %foreach.head.L45
  %76 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 0), !dbg !78
  %77 = load i32, ptr %76, align 4, !dbg !79
  %78 = icmp eq i32 %77, 123, !dbg !79
  br i1 %78, label %assert.exit.L48, label %assert.then.L48, !dbg !79, !prof !30

assert.then.L48:                                  ; preds = %foreach.exit.L45
  %79 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !79
  call void @exit(i32 1), !dbg !79
  unreachable, !dbg !79

assert.exit.L48:                                  ; preds = %foreach.exit.L45
  %80 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 1), !dbg !80
  %81 = load i32, ptr %80, align 4, !dbg !81
  %82 = icmp eq i32 %81, 4321, !dbg !81
  br i1 %82, label %assert.exit.L49, label %assert.then.L49, !dbg !81, !prof !30

assert.then.L49:                                  ; preds = %assert.exit.L48
  %83 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !81
  call void @exit(i32 1), !dbg !81
  unreachable, !dbg !81

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %84 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 2), !dbg !82
  %85 = load i32, ptr %84, align 4, !dbg !83
  %86 = icmp eq i32 %85, 9876, !dbg !83
  br i1 %86, label %assert.exit.L50, label %assert.then.L50, !dbg !83, !prof !30

assert.then.L50:                                  ; preds = %assert.exit.L49
  %87 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !83
  call void @exit(i32 1), !dbg !83
  unreachable, !dbg !83

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %88 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !84
  call void @llvm.dbg.declare(metadata ptr %item1, metadata !85, metadata !DIExpression()), !dbg !86
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %88, ptr %11, align 8, !dbg !84
  br label %foreach.head.L53, !dbg !86

foreach.head.L53:                                 ; preds = %foreach.tail.L53, %assert.exit.L50
  %89 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %11), !dbg !86
  br i1 %89, label %foreach.body.L53, label %foreach.exit.L53, !dbg !86

foreach.body.L53:                                 ; preds = %foreach.head.L53
  %90 = call ptr @_mf__VectorIterator_int__intref__get(ptr %11), !dbg !86
  store ptr %90, ptr %item1, align 8, !dbg !86
  %91 = load ptr, ptr %item1, align 8, !dbg !87
  %92 = load i32, ptr %91, align 4, !dbg !87
  %93 = add i32 %92, 1, !dbg !87
  store i32 %93, ptr %91, align 4, !dbg !87
  br label %foreach.tail.L53, !dbg !87

foreach.tail.L53:                                 ; preds = %foreach.body.L53
  call void @_mp__VectorIterator_int__void__next(ptr %11), !dbg !87
  br label %foreach.head.L53, !dbg !87

foreach.exit.L53:                                 ; preds = %foreach.head.L53
  %94 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 0), !dbg !88
  %95 = load i32, ptr %94, align 4, !dbg !89
  %96 = icmp eq i32 %95, 124, !dbg !89
  br i1 %96, label %assert.exit.L56, label %assert.then.L56, !dbg !89, !prof !30

assert.then.L56:                                  ; preds = %foreach.exit.L53
  %97 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !89
  call void @exit(i32 1), !dbg !89
  unreachable, !dbg !89

assert.exit.L56:                                  ; preds = %foreach.exit.L53
  %98 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 1), !dbg !90
  %99 = load i32, ptr %98, align 4, !dbg !91
  %100 = icmp eq i32 %99, 4322, !dbg !91
  br i1 %100, label %assert.exit.L57, label %assert.then.L57, !dbg !91, !prof !30

assert.then.L57:                                  ; preds = %assert.exit.L56
  %101 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !91
  call void @exit(i32 1), !dbg !91
  unreachable, !dbg !91

assert.exit.L57:                                  ; preds = %assert.exit.L56
  %102 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 2), !dbg !92
  %103 = load i32, ptr %102, align 4, !dbg !93
  %104 = icmp eq i32 %103, 9877, !dbg !93
  br i1 %104, label %assert.exit.L58, label %assert.then.L58, !dbg !93, !prof !30

assert.then.L58:                                  ; preds = %assert.exit.L57
  %105 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !93
  call void @exit(i32 1), !dbg !93
  unreachable, !dbg !93

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %106 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr %vi), !dbg !94
  store %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %106, ptr %12, align 8, !dbg !94
  call void @llvm.dbg.declare(metadata ptr %idx, metadata !95, metadata !DIExpression()), !dbg !97
  call void @llvm.dbg.declare(metadata ptr %item2, metadata !98, metadata !DIExpression()), !dbg !99
  store i64 0, ptr %idx, align 8, !dbg !97
  br label %foreach.head.L60, !dbg !99

foreach.head.L60:                                 ; preds = %foreach.tail.L60, %assert.exit.L58
  %107 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr %12), !dbg !99
  br i1 %107, label %foreach.body.L60, label %foreach.exit.L60, !dbg !99

foreach.body.L60:                                 ; preds = %foreach.head.L60
  %pair3 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr %12), !dbg !99
  store %__long_intref__Pair__long_intref %pair3, ptr %pair_addr, align 8, !dbg !99
  %idx_addr = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %pair_addr, i32 0, i32 0, !dbg !99
  %108 = load i64, ptr %idx_addr, align 8, !dbg !99
  store i64 %108, ptr %idx, align 8, !dbg !99
  %item_addr = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %pair_addr, i32 0, i32 1, !dbg !99
  %109 = load ptr, ptr %item_addr, align 8, !dbg !99
  store ptr %109, ptr %item2, align 8, !dbg !99
  %110 = load i64, ptr %idx, align 8, !dbg !100
  %111 = trunc i64 %110 to i32, !dbg !100
  %112 = load ptr, ptr %item2, align 8, !dbg !100
  %113 = load i32, ptr %112, align 4, !dbg !100
  %114 = add i32 %113, %111, !dbg !100
  store i32 %114, ptr %112, align 4, !dbg !100
  br label %foreach.tail.L60, !dbg !100

foreach.tail.L60:                                 ; preds = %foreach.body.L60
  call void @_mp__VectorIterator_int__void__next(ptr %12), !dbg !100
  br label %foreach.head.L60, !dbg !100

foreach.exit.L60:                                 ; preds = %foreach.head.L60
  %115 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 0), !dbg !101
  %116 = load i32, ptr %115, align 4, !dbg !102
  %117 = icmp eq i32 %116, 124, !dbg !102
  br i1 %117, label %assert.exit.L63, label %assert.then.L63, !dbg !102, !prof !30

assert.then.L63:                                  ; preds = %foreach.exit.L60
  %118 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !102
  call void @exit(i32 1), !dbg !102
  unreachable, !dbg !102

assert.exit.L63:                                  ; preds = %foreach.exit.L60
  %119 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 1), !dbg !103
  %120 = load i32, ptr %119, align 4, !dbg !104
  %121 = icmp eq i32 %120, 4323, !dbg !104
  br i1 %121, label %assert.exit.L64, label %assert.then.L64, !dbg !104, !prof !30

assert.then.L64:                                  ; preds = %assert.exit.L63
  %122 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !104
  call void @exit(i32 1), !dbg !104
  unreachable, !dbg !104

assert.exit.L64:                                  ; preds = %assert.exit.L63
  %123 = call ptr @_mf__Vector_int__intref__get__int(ptr %vi, i32 2), !dbg !105
  %124 = load i32, ptr %123, align 4, !dbg !106
  %125 = icmp eq i32 %124, 9879, !dbg !106
  br i1 %125, label %assert.exit.L65, label %assert.then.L65, !dbg !106, !prof !30

assert.then.L65:                                  ; preds = %assert.exit.L64
  %126 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !106
  call void @exit(i32 1), !dbg !106
  unreachable, !dbg !106

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %127 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !107
  %128 = load i32, ptr %result, align 4, !dbg !107
  ret i32 %128, !dbg !107
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

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}
!llvm.dbg.cu = !{!4}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 1, !"PIC Level", i32 2}
!3 = !{!"spice version dev"}
!4 = distinct !DICompileUnit(language: DW_LANG_C11, file: !5, producer: "spice version dev", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!5 = !DIFile(filename: "C:\\Users\\Marc\\Documents\\JustForFunGitHubClonesFast\\spice\\cmake-build-debug\\test\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex\\source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!6 = distinct !DISubprogram(name: "main", linkageName: "main", scope: !7, file: !7, line: 3, type: !8, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !4, retainedNodes: !14)
!7 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10, !11}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, elements: !14)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!13 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!14 = !{}
!15 = !DILocalVariable(name: "result", scope: !6, file: !7, line: 3, type: !10)
!16 = !DILocation(line: 3, column: 1, scope: !6)
!17 = !DILocalVariable(name: "argc", arg: 1, scope: !6, file: !7, line: 3, type: !10)
!18 = !DILocalVariable(name: "argv", arg: 2, scope: !6, file: !7, line: 3, type: !11)
!19 = !DILocalVariable(name: "vi", scope: !6, file: !7, line: 5, type: !20)
!20 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !7, file: !7, line: 25, align: 8, flags: DIFlagTypePassByValue, elements: !21)
!21 = !{!22, !23, !23}
!22 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!23 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
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
!34 = !{!35, !23}
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
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
!49 = !{!23, !22}
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
!74 = !DILocation(line: 45, column: 32, scope: !6)
!75 = !DILocalVariable(name: "item", scope: !6, file: !7, line: 45, type: !10)
!76 = !DILocation(line: 45, column: 13, scope: !6)
!77 = !DILocation(line: 46, column: 9, scope: !6)
!78 = !DILocation(line: 48, column: 19, scope: !6)
!79 = !DILocation(line: 48, column: 25, scope: !6)
!80 = !DILocation(line: 49, column: 19, scope: !6)
!81 = !DILocation(line: 49, column: 25, scope: !6)
!82 = !DILocation(line: 50, column: 19, scope: !6)
!83 = !DILocation(line: 50, column: 25, scope: !6)
!84 = !DILocation(line: 53, column: 33, scope: !6)
!85 = !DILocalVariable(name: "item", scope: !6, file: !7, line: 53, type: !22)
!86 = !DILocation(line: 53, column: 13, scope: !6)
!87 = !DILocation(line: 54, column: 9, scope: !6)
!88 = !DILocation(line: 56, column: 19, scope: !6)
!89 = !DILocation(line: 56, column: 25, scope: !6)
!90 = !DILocation(line: 57, column: 19, scope: !6)
!91 = !DILocation(line: 57, column: 25, scope: !6)
!92 = !DILocation(line: 58, column: 19, scope: !6)
!93 = !DILocation(line: 58, column: 25, scope: !6)
!94 = !DILocation(line: 60, column: 43, scope: !6)
!95 = !DILocalVariable(name: "idx", scope: !6, file: !7, line: 60, type: !96)
!96 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!97 = !DILocation(line: 60, column: 13, scope: !6)
!98 = !DILocalVariable(name: "item", scope: !6, file: !7, line: 60, type: !22)
!99 = !DILocation(line: 60, column: 23, scope: !6)
!100 = !DILocation(line: 61, column: 9, scope: !6)
!101 = !DILocation(line: 63, column: 19, scope: !6)
!102 = !DILocation(line: 63, column: 25, scope: !6)
!103 = !DILocation(line: 64, column: 19, scope: !6)
!104 = !DILocation(line: 64, column: 25, scope: !6)
!105 = !DILocation(line: 65, column: 19, scope: !6)
!106 = !DILocation(line: 65, column: 25, scope: !6)
!107 = !DILocation(line: 67, column: 5, scope: !6)
