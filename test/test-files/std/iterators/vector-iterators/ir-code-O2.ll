; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Vector__intptr_long_long_int = type { ptr, i64, i64, i32 }
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
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %vi = alloca %__int__Vector__intptr_long_long_int, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %it = alloca %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", align 8
  %pair = alloca %__long_intref__Pair__long_intref, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  call void @_mp__Vector_int__void__ctor(ptr nonnull %vi) #2
  store i32 123, ptr %1, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %1) #2
  store i32 4321, ptr %2, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %2) #2
  store i32 9876, ptr %3, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %3) #2
  %6 = call i64 @_mf__Vector_int__long__getSize(ptr nonnull %vi) #2
  %7 = icmp eq i64 %6, 3
  br i1 %7, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L9:                                   ; preds = %0
  %9 = call %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" @"_f__void__std/iterator/vector-iterator::VectorIterator<int>__iterate__std/iterator/vector-iterator::Vector<int>ref"(ptr nonnull %vi) #2
  %.fca.0.extract5 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %9, 0
  %.fca.1.extract7 = extractvalue %"__int__VectorIterator__std/data/vector::Vector<int>ref_long" %9, 1
  %.fca.1.gep8 = getelementptr inbounds %"__int__VectorIterator__std/data/vector::Vector<int>ref_long", ptr %it, i64 0, i32 1
  store ptr %.fca.0.extract5, ptr %it, align 8
  store i64 %.fca.1.extract7, ptr %.fca.1.gep8, align 8
  %10 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %10, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L9
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %12 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %13 = load i32, ptr %12, align 4
  %14 = icmp eq i32 %13, 123
  br i1 %14, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %16 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %17 = load i32, ptr %16, align 4
  %18 = icmp eq i32 %17, 123
  br i1 %18, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %19 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %20 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %21 = load i32, ptr %20, align 4
  %22 = icmp eq i32 %21, 4321
  br i1 %22, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L15
  %23 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %24 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %24, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %25 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %26 = call %__long_intref__Pair__long_intref @"_mf__VectorIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__getIdx"(ptr nonnull %it) #2
  %.fca.0.extract = extractvalue %__long_intref__Pair__long_intref %26, 0
  %.fca.1.extract = extractvalue %__long_intref__Pair__long_intref %26, 1
  %.fca.1.gep = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %pair, i64 0, i32 1
  store i64 %.fca.0.extract, ptr %pair, align 8
  store ptr %.fca.1.extract, ptr %.fca.1.gep, align 8
  %27 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr nonnull %pair) #2
  %28 = load i64, ptr %27, align 8
  %29 = icmp eq i64 %28, 2
  br i1 %29, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L18
  %30 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %31 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr nonnull %pair) #2
  %32 = load i32, ptr %31, align 4
  %33 = icmp eq i32 %32, 9876
  br i1 %33, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L21
  %34 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %35 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %35, label %assert.then.L24, label %assert.exit.L24, !prof !1

assert.then.L24:                                  ; preds = %assert.exit.L22
  %36 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %4, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %4) #2
  store i32 -99, ptr %5, align 4
  call void @_mp__Vector_int__void__pushBack__intref(ptr nonnull %vi, ptr nonnull %5) #2
  %37 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %37, label %assert.exit.L29, label %assert.then.L29, !prof !0

assert.then.L29:                                  ; preds = %assert.exit.L24
  %38 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @"_p__void__void__op.minusequal__VectorIterator<int>ref_int"(ptr nonnull %it, i32 3) #2
  %39 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %40 = load i32, ptr %39, align 4
  %41 = icmp eq i32 %40, 123
  br i1 %41, label %assert.exit.L33, label %assert.then.L33, !prof !0

assert.then.L33:                                  ; preds = %assert.exit.L29
  %42 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %43 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %43, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L33
  %44 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L33
  call void @"_p__void__void__op.plusplus.post__VectorIterator<int>ref"(ptr nonnull %it) #2
  %45 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %46 = load i32, ptr %45, align 4
  %47 = icmp eq i32 %46, 4321
  br i1 %47, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L34
  %48 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L34
  call void @"_p__void__void__op.minusminus.post__VectorIterator<int>ref"(ptr nonnull %it) #2
  %49 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %50 = load i32, ptr %49, align 4
  %51 = icmp eq i32 %50, 123
  br i1 %51, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %assert.exit.L36
  %52 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @"_p__void__void__op.plusequal__VectorIterator<int>ref_int"(ptr nonnull %it, i32 4) #2
  %53 = call ptr @_mf__VectorIterator_int__intref__get(ptr nonnull %it) #2
  %54 = load i32, ptr %53, align 4
  %55 = icmp eq i32 %54, -99
  br i1 %55, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %assert.exit.L38
  %56 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_mp__VectorIterator_int__void__next(ptr nonnull %it) #2
  %57 = call i1 @_mf__VectorIterator_int__bool__isValid(ptr nonnull %it) #2
  br i1 %57, label %assert.then.L42, label %assert.exit.L42, !prof !1

assert.then.L42:                                  ; preds = %assert.exit.L40
  %58 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %59 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
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

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
