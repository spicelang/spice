; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"_s__pair.Pair<int,string>__Vector__pair.Pair<int,string>ptr_long_long_int" = type { ptr, i64, i64, i32 }
%_s__int_string__Pair__int_string = type { i32, ptr }

@0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@2 = private unnamed_addr constant [11 x i8] c"Hello %s!\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l4:
  %pairVector = alloca %"_s__pair.Pair<int,string>__Vector__pair.Pair<int,string>ptr_long_long_int", align 8
  %0 = alloca %_s__int_string__Pair__int_string, align 8
  %1 = alloca %_s__int_string__Pair__int_string, align 8
  %2 = alloca %_s__int_string__Pair__int_string, align 8
  call void @"_mp__Vector_pair.Pair<int,string>__void__ctor"(ptr nonnull %pairVector)
  call void @_mp__Pair_int_string__void__ctor__int_string(ptr nonnull %0, i32 0, ptr nonnull @0)
  %.fca.0.load7 = load i32, ptr %0, align 8
  %.fca.0.insert8 = insertvalue %_s__int_string__Pair__int_string poison, i32 %.fca.0.load7, 0
  %.fca.1.gep9 = getelementptr inbounds %_s__int_string__Pair__int_string, ptr %0, i64 0, i32 1
  %.fca.1.load10 = load ptr, ptr %.fca.1.gep9, align 8
  %.fca.1.insert11 = insertvalue %_s__int_string__Pair__int_string %.fca.0.insert8, ptr %.fca.1.load10, 1
  call void @"_mp__Vector_pair.Pair<int,string>__void__pushBack__pair.Pair<int,string>"(ptr nonnull %pairVector, %_s__int_string__Pair__int_string %.fca.1.insert11)
  call void @_mp__Pair_int_string__void__ctor__int_string(ptr nonnull %1, i32 1, ptr nonnull @1)
  %.fca.0.load = load i32, ptr %1, align 8
  %.fca.0.insert = insertvalue %_s__int_string__Pair__int_string poison, i32 %.fca.0.load, 0
  %.fca.1.gep3 = getelementptr inbounds %_s__int_string__Pair__int_string, ptr %1, i64 0, i32 1
  %.fca.1.load = load ptr, ptr %.fca.1.gep3, align 8
  %.fca.1.insert = insertvalue %_s__int_string__Pair__int_string %.fca.0.insert, ptr %.fca.1.load, 1
  call void @"_mp__Vector_pair.Pair<int,string>__void__pushBack__pair.Pair<int,string>"(ptr nonnull %pairVector, %_s__int_string__Pair__int_string %.fca.1.insert)
  %3 = call %_s__int_string__Pair__int_string @"_mf__Vector_pair.Pair<int,string>__pair.Pair<int,string>__get__int"(ptr nonnull %pairVector, i32 1)
  %.fca.0.extract = extractvalue %_s__int_string__Pair__int_string %3, 0
  store i32 %.fca.0.extract, ptr %2, align 8
  %.fca.1.extract = extractvalue %_s__int_string__Pair__int_string %3, 1
  %.fca.1.gep = getelementptr inbounds %_s__int_string__Pair__int_string, ptr %2, i64 0, i32 1
  store ptr %.fca.1.extract, ptr %.fca.1.gep, align 8
  %4 = call ptr @_mf__Pair_int_string__string__getSecond(ptr nonnull %2)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull @2, ptr %4)
  call void @"_mp__Vector_pair.Pair<int,string>__void__dtor"(ptr nonnull %pairVector)
  ret i32 0
}

declare void @"_mp__Vector_pair.Pair<int,string>__void__ctor"(ptr) local_unnamed_addr

declare void @"_mp__Vector_pair.Pair<int,string>__void__pushBack__pair.Pair<int,string>"(ptr, %_s__int_string__Pair__int_string) local_unnamed_addr

declare void @_mp__Pair_int_string__void__ctor__int_string(ptr, i32, ptr) local_unnamed_addr

declare %_s__int_string__Pair__int_string @"_mf__Vector_pair.Pair<int,string>__pair.Pair<int,string>__get__int"(ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare ptr @_mf__Pair_int_string__string__getSecond(ptr) local_unnamed_addr

declare void @"_mp__Vector_pair.Pair<int,string>__void__dtor"(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
