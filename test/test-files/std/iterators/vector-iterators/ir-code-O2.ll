; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Vector__intptr_long_long = type { ptr, i64, i64 }
%"__int__VectorIterator__std/data/vector::Vector<int>ref_long" = type { ptr, i64 }
%__long_intref__Pair__long_intref = type { i64, ptr }

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.getSize() == 3' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 123' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4321' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9876' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4322' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9877' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4323' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9879' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %vi = alloca %__int__Vector__intptr_long_long, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %it = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %pair = alloca %__long_intref__Pair__long_intref, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %7 = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %8 = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  call void @_mp__Vector_int__void__ctor(ptr nonnull %vi) #2
  store i32 123, ptr %1, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %1) #2
  store i32 4321, ptr %2, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %2) #2
  store i32 9876, ptr %3, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %3) #2
  %9 = call i64 @_mf__Vector_int__long__getSize(ptr nonnull %vi) #2
  %10 = icmp eq i64 %9, 3
  br i1 %10, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %0
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L9:                                   ; preds = %0
  %12 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr nonnull %vi) #2
  %.fca.0.extract20 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %12, 0
  %.fca.1.extract22 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %12, 1
  %.fca.1.gep23 = getelementptr inbounds %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, i64 0, i32 1
  store ptr %.fca.0.extract20, ptr %it, align 8
  store i64 %.fca.1.extract22, ptr %.fca.1.gep23, align 8
  %13 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %13, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L9
  %14 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %15 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %16 = load i32, ptr %15, align 4
  %17 = icmp eq i32 %16, 123
  br i1 %17, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %18 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %19 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %20 = load i32, ptr %19, align 4
  %21 = icmp eq i32 %20, 123
  br i1 %21, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %22 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %23 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %24 = load i32, ptr %23, align 4
  %25 = icmp eq i32 %24, 4321
  br i1 %25, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L15
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %27 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %27, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %28 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %29 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr nonnull %it) #2
  %.fca.0.extract12 = extractvalue %__long_intref__Pair__long_intref %29, 0
  %.fca.1.extract14 = extractvalue %__long_intref__Pair__long_intref %29, 1
  %.fca.1.gep15 = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %pair, i64 0, i32 1
  store i64 %.fca.0.extract12, ptr %pair, align 8
  store ptr %.fca.1.extract14, ptr %.fca.1.gep15, align 8
  %30 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr nonnull %pair) #2
  %31 = load i64, ptr %30, align 8
  %32 = icmp eq i64 %31, 2
  br i1 %32, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L18
  %33 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %34 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr nonnull %pair) #2
  %35 = load i32, ptr %34, align 4
  %36 = icmp eq i32 %35, 9876
  br i1 %36, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L21
  %37 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %38 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %38, label %assert.then.L24, label %assert.exit.L24, !prof !1

assert.then.L24:                                  ; preds = %assert.exit.L22
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %4, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %4) #2
  store i32 -99, ptr %5, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %5) #2
  %40 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %40, label %assert.exit.L29, label %assert.then.L29, !prof !0

assert.then.L29:                                  ; preds = %assert.exit.L24
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr nonnull %it, i32 3) #2
  %42 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %43 = load i32, ptr %42, align 4
  %44 = icmp eq i32 %43, 123
  br i1 %44, label %assert.exit.L33, label %assert.then.L33, !prof !0

assert.then.L33:                                  ; preds = %assert.exit.L29
  %45 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %46 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %46, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L33
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  call void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr nonnull %it) #2
  %48 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %49 = load i32, ptr %48, align 4
  %50 = icmp eq i32 %49, 4321
  br i1 %50, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L34
  %51 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L34
  call void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr nonnull %it) #2
  %52 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %53 = load i32, ptr %52, align 4
  %54 = icmp eq i32 %53, 123
  br i1 %54, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L36
  %55 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr nonnull %it, i32 4) #2
  %56 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %57 = load i32, ptr %56, align 4
  %58 = icmp eq i32 %57, -99
  br i1 %58, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %assert.exit.L38
  %59 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %60 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %60, label %assert.then.L42, label %assert.exit.L42, !prof !1

assert.then.L42:                                  ; preds = %assert.exit.L40
  %61 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %62 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr nonnull %vi) #2
  %.fca.0.extract8 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %62, 0
  store ptr %.fca.0.extract8, ptr %6, align 8
  %.fca.1.extract10 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %62, 1
  %.fca.1.gep11 = getelementptr inbounds %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %6, i64 0, i32 1
  store i64 %.fca.1.extract10, ptr %.fca.1.gep11, align 8
  %63 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %6) #2
  br i1 %63, label %foreach.body.L45, label %foreach.exit.L45

foreach.body.L45:                                 ; preds = %assert.exit.L42, %foreach.body.L45
  %64 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %6) #2
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %6) #2
  %65 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %6) #2
  br i1 %65, label %foreach.body.L45, label %foreach.exit.L45

foreach.exit.L45:                                 ; preds = %foreach.body.L45, %assert.exit.L42
  %66 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 0) #2
  %67 = load i32, ptr %66, align 4
  %68 = icmp eq i32 %67, 123
  br i1 %68, label %assert.exit.L48, label %assert.then.L48, !prof !0

assert.then.L48:                                  ; preds = %foreach.exit.L45
  %69 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.16)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L48:                                  ; preds = %foreach.exit.L45
  %70 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 1) #2
  %71 = load i32, ptr %70, align 4
  %72 = icmp eq i32 %71, 4321
  br i1 %72, label %assert.exit.L49, label %assert.then.L49, !prof !0

assert.then.L49:                                  ; preds = %assert.exit.L48
  %73 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %74 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 2) #2
  %75 = load i32, ptr %74, align 4
  %76 = icmp eq i32 %75, 9876
  br i1 %76, label %assert.exit.L50, label %assert.then.L50, !prof !0

assert.then.L50:                                  ; preds = %assert.exit.L49
  %77 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.18)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %78 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr nonnull %vi) #2
  %.fca.0.extract4 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %78, 0
  store ptr %.fca.0.extract4, ptr %7, align 8
  %.fca.1.extract6 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %78, 1
  %.fca.1.gep7 = getelementptr inbounds %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %7, i64 0, i32 1
  store i64 %.fca.1.extract6, ptr %.fca.1.gep7, align 8
  %79 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %7) #2
  br i1 %79, label %foreach.body.L53, label %foreach.exit.L53

foreach.body.L53:                                 ; preds = %assert.exit.L50, %foreach.body.L53
  %80 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %7) #2
  %81 = load i32, ptr %80, align 4
  %82 = add i32 %81, 1
  store i32 %82, ptr %80, align 4
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %7) #2
  %83 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %7) #2
  br i1 %83, label %foreach.body.L53, label %foreach.exit.L53

foreach.exit.L53:                                 ; preds = %foreach.body.L53, %assert.exit.L50
  %84 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 0) #2
  %85 = load i32, ptr %84, align 4
  %86 = icmp eq i32 %85, 124
  br i1 %86, label %assert.exit.L56, label %assert.then.L56, !prof !0

assert.then.L56:                                  ; preds = %foreach.exit.L53
  %87 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.22)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L56:                                  ; preds = %foreach.exit.L53
  %88 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 1) #2
  %89 = load i32, ptr %88, align 4
  %90 = icmp eq i32 %89, 4322
  br i1 %90, label %assert.exit.L57, label %assert.then.L57, !prof !0

assert.then.L57:                                  ; preds = %assert.exit.L56
  %91 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.20)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L57:                                  ; preds = %assert.exit.L56
  %92 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 2) #2
  %93 = load i32, ptr %92, align 4
  %94 = icmp eq i32 %93, 9877
  br i1 %94, label %assert.exit.L58, label %assert.then.L58, !prof !0

assert.then.L58:                                  ; preds = %assert.exit.L57
  %95 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %96 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr nonnull %vi) #2
  %.fca.0.extract = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %96, 0
  store ptr %.fca.0.extract, ptr %8, align 8
  %.fca.1.extract = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %96, 1
  %.fca.1.gep = getelementptr inbounds %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %8, i64 0, i32 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %97 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %8) #2
  br i1 %97, label %foreach.body.L60, label %foreach.exit.L60

foreach.body.L60:                                 ; preds = %assert.exit.L58, %foreach.body.L60
  %pair3 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr nonnull %8) #2
  %pair3.fca.0.extract = extractvalue %__long_intref__Pair__long_intref %pair3, 0
  %pair3.fca.1.extract = extractvalue %__long_intref__Pair__long_intref %pair3, 1
  %98 = trunc i64 %pair3.fca.0.extract to i32
  %99 = load i32, ptr %pair3.fca.1.extract, align 4
  %100 = add i32 %99, %98
  store i32 %100, ptr %pair3.fca.1.extract, align 4
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %8) #2
  %101 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %8) #2
  br i1 %101, label %foreach.body.L60, label %foreach.exit.L60

foreach.exit.L60:                                 ; preds = %foreach.body.L60, %assert.exit.L58
  %102 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 0) #2
  %103 = load i32, ptr %102, align 4
  %104 = icmp eq i32 %103, 124
  br i1 %104, label %assert.exit.L63, label %assert.then.L63, !prof !0

assert.then.L63:                                  ; preds = %foreach.exit.L60
  %105 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.22)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L63:                                  ; preds = %foreach.exit.L60
  %106 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 1) #2
  %107 = load i32, ptr %106, align 4
  %108 = icmp eq i32 %107, 4323
  br i1 %108, label %assert.exit.L64, label %assert.then.L64, !prof !0

assert.then.L64:                                  ; preds = %assert.exit.L63
  %109 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L64:                                  ; preds = %assert.exit.L63
  %110 = call ptr @_mf__Vector_int__intref__get__int(ptr nonnull %vi, i32 2) #2
  %111 = load i32, ptr %110, align 4
  %112 = icmp eq i32 %111, 9879
  br i1 %112, label %assert.exit.L65, label %assert.then.L65, !prof !0

assert.then.L65:                                  ; preds = %assert.exit.L64
  %113 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %114 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare void @_mp__Vector_int__void__ctor(ptr) local_unnamed_addr

declare void @_mp__Vector_int__void__pushBack__intref(ptr, ptr) local_unnamed_addr

declare i64 @_mf__Vector_int__long__getSize(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr) local_unnamed_addr

declare i1 @_mf__VectorIterator_int__bool__isValid(ptr) local_unnamed_addr

declare ptr @_mf__VectorIterator_int__intref__get(ptr) local_unnamed_addr

declare void @_mp__VectorIterator_int__void__next(ptr) local_unnamed_addr

declare %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr) local_unnamed_addr

declare ptr @_mf__Pair_long_intref__longref__getFirst(ptr) local_unnamed_addr

declare ptr @_mf__Pair_long_intref__intref__getSecond(ptr) local_unnamed_addr

declare void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr, i32) local_unnamed_addr

declare void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr) local_unnamed_addr

declare void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr) local_unnamed_addr

declare void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr, i32) local_unnamed_addr

declare ptr @_mf__Vector_int__intref__get__int(ptr, i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
