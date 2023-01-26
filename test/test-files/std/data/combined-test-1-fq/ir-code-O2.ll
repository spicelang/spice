; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"__pair::Pair<int,string>__Vector__pair::Pair<int,string>ptr_long_long_int" = type { ptr, i64, i64, i32 }
%__int_string__Pair__int_string = type { i32, ptr }

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@printf.str.0 = private unnamed_addr constant [11 x i8] c"Hello %s!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %pairVector = alloca %"__pair::Pair<int,string>__Vector__pair::Pair<int,string>ptr_long_long_int", align 8
  %1 = alloca %__int_string__Pair__int_string, align 8
  %2 = alloca %__int_string__Pair__int_string, align 8
  %p1 = alloca %__int_string__Pair__int_string, align 8
  call void @"_mp__Vector_pair::Pair<int,string>__void__ctor"(ptr nonnull %pairVector) #2
  call void @_mp__Pair_int_string__void__ctor__int_string(ptr nonnull %1, i32 0, ptr nonnull @anon.string.0) #2
  %.fca.0.load4 = load i32, ptr %1, align 8
  %.fca.0.insert5 = insertvalue %__int_string__Pair__int_string poison, i32 %.fca.0.load4, 0
  %.fca.1.gep6 = getelementptr inbounds %__int_string__Pair__int_string, ptr %1, i64 0, i32 1
  %.fca.1.load7 = load ptr, ptr %.fca.1.gep6, align 8
  %.fca.1.insert8 = insertvalue %__int_string__Pair__int_string %.fca.0.insert5, ptr %.fca.1.load7, 1
  call void @"_mp__Vector_pair::Pair<int,string>__void__pushBack__pair::Pair<int,string>"(ptr nonnull %pairVector, %__int_string__Pair__int_string %.fca.1.insert8) #2
  call void @_mp__Pair_int_string__void__ctor__int_string(ptr nonnull %2, i32 1, ptr nonnull @anon.string.1) #2
  %.fca.0.load = load i32, ptr %2, align 8
  %.fca.0.insert = insertvalue %__int_string__Pair__int_string poison, i32 %.fca.0.load, 0
  %.fca.1.gep2 = getelementptr inbounds %__int_string__Pair__int_string, ptr %2, i64 0, i32 1
  %.fca.1.load = load ptr, ptr %.fca.1.gep2, align 8
  %.fca.1.insert = insertvalue %__int_string__Pair__int_string %.fca.0.insert, ptr %.fca.1.load, 1
  call void @"_mp__Vector_pair::Pair<int,string>__void__pushBack__pair::Pair<int,string>"(ptr nonnull %pairVector, %__int_string__Pair__int_string %.fca.1.insert) #2
  %3 = call %__int_string__Pair__int_string @"_mf__Vector_pair::Pair<int,string>__pair::Pair<int,string>__get__int"(ptr nonnull %pairVector, i32 1) #2
  %.fca.0.extract = extractvalue %__int_string__Pair__int_string %3, 0
  store i32 %.fca.0.extract, ptr %p1, align 8
  %.fca.1.extract = extractvalue %__int_string__Pair__int_string %3, 1
  %.fca.1.gep = getelementptr inbounds %__int_string__Pair__int_string, ptr %p1, i64 0, i32 1
  store ptr %.fca.1.extract, ptr %.fca.1.gep, align 8
  %4 = call ptr @_mf__Pair_int_string__string__getSecond(ptr nonnull %p1) #2
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %4)
  ret i32 0
}

declare void @"_mp__Vector_pair::Pair<int,string>__void__ctor"(ptr) local_unnamed_addr

declare void @_mp__Pair_int_string__void__ctor__int_string(ptr, i32, ptr) local_unnamed_addr

declare void @"_mp__Vector_pair::Pair<int,string>__void__pushBack__pair::Pair<int,string>"(ptr, %__int_string__Pair__int_string) local_unnamed_addr

declare %__int_string__Pair__int_string @"_mf__Vector_pair::Pair<int,string>__pair::Pair<int,string>__get__int"(ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_mf__Pair_int_string__string__getSecond(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
