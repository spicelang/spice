; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__NumberIterator__int_int_int = type { i32, i32, i32 }
%__long_intref__Pair__long_intref = type { i64, ptr }
%__long__NumberIterator__long_long_long = type { i64, i64, i64 }
%__long_longref__Pair__long_longref = type { i64, ptr }

@anon.string.1 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 1' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'itInt.next() == 2' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 3' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [82 x i8] c"Assertion failed: Condition 'idxAndValueInt.getFirst() == 3l' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [82 x i8] c"Assertion failed: Condition 'idxAndValueInt.getSecond() == 4' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 5' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 4' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'itInt.hasNext()' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'itInt.get() == 10' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition '!itInt.hasNext()' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 6l' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.next() == 7l' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 10l' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 8l' evaluated to false.\00", align 1
@anon.string.19 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 16l' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [84 x i8] c"Assertion failed: Condition 'idxAndValueLong.getFirst() == 11l' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [85 x i8] c"Assertion failed: Condition 'idxAndValueLong.getSecond() == 17l' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 18l' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 17l' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itLong.hasNext()' evaluated to false.\00", align 1
@anon.string.25 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 45' evaluated to false.\00", align 1
@anon.string.26 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition '!itLong.hasNext()' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %itInt = alloca %__int__NumberIterator__int_int_int, align 8
  %idxAndValueInt = alloca %__long_intref__Pair__long_intref, align 8
  %itLong = alloca %__long__NumberIterator__long_long_long, align 8
  %idxAndValueLong = alloca %__long_longref__Pair__long_longref, align 8
  %1 = tail call %__int__NumberIterator__int_int_int @"_f__void__NumberIterator<int>__range__int_int"(i32 1, i32 10) #2
  %.fca.0.extract35 = extractvalue %__int__NumberIterator__int_int_int %1, 0
  %.fca.1.extract37 = extractvalue %__int__NumberIterator__int_int_int %1, 1
  %.fca.1.gep38 = getelementptr inbounds %__int__NumberIterator__int_int_int, ptr %itInt, i64 0, i32 1
  %.fca.2.extract39 = extractvalue %__int__NumberIterator__int_int_int %1, 2
  %.fca.2.gep40 = getelementptr inbounds %__int__NumberIterator__int_int_int, ptr %itInt, i64 0, i32 2
  store i32 %.fca.0.extract35, ptr %itInt, align 8
  store i32 %.fca.1.extract37, ptr %.fca.1.gep38, align 4
  store i32 %.fca.2.extract39, ptr %.fca.2.gep40, align 8
  %2 = call i1 @_mf__NumberIterator_int__bool__hasNext(ptr nonnull %itInt) #2
  br i1 %2, label %assert.exit.L8, label %assert.then.L8, !prof !0

assert.then.L8:                                   ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L8:                                   ; preds = %0
  %4 = call ptr @_mf__NumberIterator_int__intref__get(ptr nonnull %itInt) #2
  %5 = load i32, ptr %4, align 4
  %6 = icmp eq i32 %5, 1
  br i1 %6, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %assert.exit.L8
  %7 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L8
  %8 = call ptr @_mf__NumberIterator_int__intref__next(ptr nonnull %itInt) #2
  %9 = load i32, ptr %8, align 4
  %10 = icmp eq i32 %9, 2
  br i1 %10, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L9
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  call void @"_p__void__void__op.plusequal__NumberIterator<int>ref_int"(ptr nonnull %itInt, i32 3) #2
  %12 = call ptr @_mf__NumberIterator_int__intref__get(ptr nonnull %itInt) #2
  %13 = load i32, ptr %12, align 4
  %14 = icmp eq i32 %13, 5
  br i1 %14, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L10
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L10
  %16 = call i1 @_mf__NumberIterator_int__bool__hasNext(ptr nonnull %itInt) #2
  br i1 %16, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L12
  %17 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L12
  call void @"_p__void__void__op.minusequal__NumberIterator<int>ref_int"(ptr nonnull %itInt, i32 2) #2
  %18 = call ptr @_mf__NumberIterator_int__intref__get(ptr nonnull %itInt) #2
  %19 = load i32, ptr %18, align 4
  %20 = icmp eq i32 %19, 3
  br i1 %20, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L13
  %21 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %22 = call %__long_intref__Pair__long_intref @"_mf__NumberIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__nextIdx"(ptr nonnull %itInt) #2
  %.fca.0.extract27 = extractvalue %__long_intref__Pair__long_intref %22, 0
  %.fca.1.extract29 = extractvalue %__long_intref__Pair__long_intref %22, 1
  %.fca.1.gep30 = getelementptr inbounds %__long_intref__Pair__long_intref, ptr %idxAndValueInt, i64 0, i32 1
  store i64 %.fca.0.extract27, ptr %idxAndValueInt, align 8
  store ptr %.fca.1.extract29, ptr %.fca.1.gep30, align 8
  %23 = call ptr @_mf__Pair_long_intref__longref__getFirst(ptr nonnull %idxAndValueInt) #2
  %24 = load i64, ptr %23, align 8
  %25 = icmp eq i64 %24, 3
  br i1 %25, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L15
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %27 = call ptr @_mf__Pair_long_intref__intref__getSecond(ptr nonnull %idxAndValueInt) #2
  %28 = load i32, ptr %27, align 4
  %29 = icmp eq i32 %28, 4
  br i1 %29, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %30 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @"_p__void__void__op.plusplus.post__NumberIterator<int>ref"(ptr nonnull %itInt) #2
  %31 = call ptr @_mf__NumberIterator_int__intref__get(ptr nonnull %itInt) #2
  %32 = load i32, ptr %31, align 4
  %33 = icmp eq i32 %32, 5
  br i1 %33, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L18
  %34 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L18
  call void @"_p__void__void__op.minusminus.post__NumberIterator<int>ref"(ptr nonnull %itInt) #2
  %35 = call ptr @_mf__NumberIterator_int__intref__get(ptr nonnull %itInt) #2
  %36 = load i32, ptr %35, align 4
  %37 = icmp eq i32 %36, 4
  br i1 %37, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L20
  %38 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L20
  %39 = call i1 @_mf__NumberIterator_int__bool__hasNext(ptr nonnull %itInt) #2
  br i1 %39, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L22
  %40 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L22
  call void @"_p__void__void__op.plusequal__NumberIterator<int>ref_int"(ptr nonnull %itInt, i32 6) #2
  %41 = call ptr @_mf__NumberIterator_int__intref__get(ptr nonnull %itInt) #2
  %42 = load i32, ptr %41, align 4
  %43 = icmp eq i32 %42, 10
  br i1 %43, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L23
  %44 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L23
  %45 = call i1 @_mf__NumberIterator_int__bool__hasNext(ptr nonnull %itInt) #2
  br i1 %45, label %assert.then.L26, label %assert.exit.L26, !prof !1

assert.then.L26:                                  ; preds = %assert.exit.L25
  %46 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L25
  %47 = call %__long__NumberIterator__long_long_long @"_f__void__NumberIterator<long>__range__long_long"(i64 6, i64 45) #2
  %.fca.0.extract5 = extractvalue %__long__NumberIterator__long_long_long %47, 0
  %.fca.1.extract7 = extractvalue %__long__NumberIterator__long_long_long %47, 1
  %.fca.1.gep8 = getelementptr inbounds %__long__NumberIterator__long_long_long, ptr %itLong, i64 0, i32 1
  %.fca.2.extract = extractvalue %__long__NumberIterator__long_long_long %47, 2
  %.fca.2.gep = getelementptr inbounds %__long__NumberIterator__long_long_long, ptr %itLong, i64 0, i32 2
  store i64 %.fca.0.extract5, ptr %itLong, align 8
  store i64 %.fca.1.extract7, ptr %.fca.1.gep8, align 8
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %48 = call i1 @_mf__NumberIterator_long__bool__hasNext(ptr nonnull %itLong) #2
  br i1 %48, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L26
  %49 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L26
  %50 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %51 = load i64, ptr %50, align 8
  %52 = icmp eq i64 %51, 6
  br i1 %52, label %assert.exit.L31, label %assert.then.L31, !prof !0

assert.then.L31:                                  ; preds = %assert.exit.L30
  %53 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L31:                                  ; preds = %assert.exit.L30
  %54 = call ptr @_mf__NumberIterator_long__longref__next(ptr nonnull %itLong) #2
  %55 = load i64, ptr %54, align 8
  %56 = icmp eq i64 %55, 7
  br i1 %56, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L31
  %57 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L31
  call void @"_p__void__void__op.plusequal__NumberIterator<long>ref_long"(ptr nonnull %itLong, i64 3) #2
  %58 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %59 = load i64, ptr %58, align 8
  %60 = icmp eq i64 %59, 10
  br i1 %60, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L32
  %61 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L32
  %62 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %63 = load i64, ptr %62, align 8
  %64 = icmp eq i64 %63, 10
  br i1 %64, label %assert.exit.L35, label %assert.then.L35, !prof !0

assert.then.L35:                                  ; preds = %assert.exit.L34
  %65 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L35:                                  ; preds = %assert.exit.L34
  call void @"_p__void__void__op.minusequal__NumberIterator<long>ref_long"(ptr nonnull %itLong, i64 2) #2
  %66 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %67 = load i64, ptr %66, align 8
  %68 = icmp eq i64 %67, 8
  br i1 %68, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L35
  %69 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.18)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L35
  call void @"_p__void__void__op.plusequal__NumberIterator<long>ref_long"(ptr nonnull %itLong, i64 8) #2
  %70 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %71 = load i64, ptr %70, align 8
  %72 = icmp eq i64 %71, 16
  br i1 %72, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L37
  %73 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L37
  %74 = call %__long_longref__Pair__long_longref @"_mf__NumberIterator_long__std/iterator/iterable::Pair<unsigned long,long&>__nextIdx"(ptr nonnull %itLong) #2
  %.fca.0.extract = extractvalue %__long_longref__Pair__long_longref %74, 0
  %.fca.1.extract = extractvalue %__long_longref__Pair__long_longref %74, 1
  %.fca.1.gep = getelementptr inbounds %__long_longref__Pair__long_longref, ptr %idxAndValueLong, i64 0, i32 1
  store i64 %.fca.0.extract, ptr %idxAndValueLong, align 8
  store ptr %.fca.1.extract, ptr %.fca.1.gep, align 8
  %75 = call ptr @_mf__Pair_long_longref__longref__getFirst(ptr nonnull %idxAndValueLong) #2
  %76 = load i64, ptr %75, align 8
  %77 = icmp eq i64 %76, 11
  br i1 %77, label %assert.exit.L41, label %assert.then.L41, !prof !0

assert.then.L41:                                  ; preds = %assert.exit.L39
  %78 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.20)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L39
  %79 = call ptr @_mf__Pair_long_longref__longref__getSecond(ptr nonnull %idxAndValueLong) #2
  %80 = load i64, ptr %79, align 8
  %81 = icmp eq i64 %80, 17
  br i1 %81, label %assert.exit.L42, label %assert.then.L42, !prof !0

assert.then.L42:                                  ; preds = %assert.exit.L41
  %82 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L41
  call void @"_p__void__void__op.plusplus.post__NumberIterator<long>ref"(ptr nonnull %itLong) #2
  %83 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %84 = load i64, ptr %83, align 8
  %85 = icmp eq i64 %84, 18
  br i1 %85, label %assert.exit.L44, label %assert.then.L44, !prof !0

assert.then.L44:                                  ; preds = %assert.exit.L42
  %86 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.22)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L44:                                  ; preds = %assert.exit.L42
  call void @"_p__void__void__op.minusminus.post__NumberIterator<long>ref"(ptr nonnull %itLong) #2
  %87 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %88 = load i64, ptr %87, align 8
  %89 = icmp eq i64 %88, 17
  br i1 %89, label %assert.exit.L46, label %assert.then.L46, !prof !0

assert.then.L46:                                  ; preds = %assert.exit.L44
  %90 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L46:                                  ; preds = %assert.exit.L44
  %91 = call i1 @_mf__NumberIterator_long__bool__hasNext(ptr nonnull %itLong) #2
  br i1 %91, label %assert.exit.L47, label %assert.then.L47, !prof !0

assert.then.L47:                                  ; preds = %assert.exit.L46
  %92 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L47:                                  ; preds = %assert.exit.L46
  call void @"_p__void__void__op.plusequal__NumberIterator<long>ref_long"(ptr nonnull %itLong, i64 28) #2
  %93 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %itLong) #2
  %94 = load i64, ptr %93, align 8
  %95 = icmp eq i64 %94, 45
  br i1 %95, label %assert.exit.L49, label %assert.then.L49, !prof !0

assert.then.L49:                                  ; preds = %assert.exit.L47
  %96 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.25)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L49:                                  ; preds = %assert.exit.L47
  %97 = call i1 @_mf__NumberIterator_long__bool__hasNext(ptr nonnull %itLong) #2
  br i1 %97, label %assert.then.L50, label %assert.exit.L50, !prof !1

assert.then.L50:                                  ; preds = %assert.exit.L49
  %98 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.26)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %99 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare %__int__NumberIterator__int_int_int @"_f__void__NumberIterator<int>__range__int_int"(i32, i32) local_unnamed_addr

declare i1 @_mf__NumberIterator_int__bool__hasNext(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare ptr @_mf__NumberIterator_int__intref__get(ptr) local_unnamed_addr

declare ptr @_mf__NumberIterator_int__intref__next(ptr) local_unnamed_addr

declare void @"_p__void__void__op.plusequal__NumberIterator<int>ref_int"(ptr, i32) local_unnamed_addr

declare void @"_p__void__void__op.minusequal__NumberIterator<int>ref_int"(ptr, i32) local_unnamed_addr

declare %__long_intref__Pair__long_intref @"_mf__NumberIterator_int__std/iterator/iterable::Pair<unsigned long,int&>__nextIdx"(ptr) local_unnamed_addr

declare ptr @_mf__Pair_long_intref__longref__getFirst(ptr) local_unnamed_addr

declare ptr @_mf__Pair_long_intref__intref__getSecond(ptr) local_unnamed_addr

declare void @"_p__void__void__op.plusplus.post__NumberIterator<int>ref"(ptr) local_unnamed_addr

declare void @"_p__void__void__op.minusminus.post__NumberIterator<int>ref"(ptr) local_unnamed_addr

declare %__long__NumberIterator__long_long_long @"_f__void__NumberIterator<long>__range__long_long"(i64, i64) local_unnamed_addr

declare i1 @_mf__NumberIterator_long__bool__hasNext(ptr) local_unnamed_addr

declare ptr @_mf__NumberIterator_long__longref__get(ptr) local_unnamed_addr

declare ptr @_mf__NumberIterator_long__longref__next(ptr) local_unnamed_addr

declare void @"_p__void__void__op.plusequal__NumberIterator<long>ref_long"(ptr, i64) local_unnamed_addr

declare void @"_p__void__void__op.minusequal__NumberIterator<long>ref_long"(ptr, i64) local_unnamed_addr

declare %__long_longref__Pair__long_longref @"_mf__NumberIterator_long__std/iterator/iterable::Pair<unsigned long,long&>__nextIdx"(ptr) local_unnamed_addr

declare ptr @_mf__Pair_long_longref__longref__getFirst(ptr) local_unnamed_addr

declare ptr @_mf__Pair_long_longref__longref__getSecond(ptr) local_unnamed_addr

declare void @"_p__void__void__op.plusplus.post__NumberIterator<long>ref"(ptr) local_unnamed_addr

declare void @"_p__void__void__op.minusminus.post__NumberIterator<long>ref"(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
