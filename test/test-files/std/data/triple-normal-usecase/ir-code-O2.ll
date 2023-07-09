; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Triple = type { ptr, i32, i1 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@printf.str.0 = private unnamed_addr constant [11 x i8] c"First: %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Second: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [11 x i8] c"Third: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %stringIntBoolTriple = alloca %struct.Triple, align 8
  %1 = alloca ptr, align 8
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store ptr @anon.string.0, ptr %1, align 8
  store i32 1234, ptr %2, align 4
  store i1 true, ptr %3, align 1
  call void @_ZN6TripleIPcibE4ctorERPcRiRb(ptr nonnull %stringIntBoolTriple, ptr nonnull %1, ptr nonnull %2, ptr nonnull %3) #2
  %4 = call ptr @_ZN6TripleIPcibE8getFirstEv(ptr nonnull %stringIntBoolTriple) #2
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %5)
  %7 = call ptr @_ZN6TripleIPcibE9getSecondEv(ptr nonnull %stringIntBoolTriple) #2
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %8)
  %10 = call ptr @_ZN6TripleIPcibE8getThirdEv(ptr nonnull %stringIntBoolTriple) #2
  %11 = load i1, ptr %10, align 1
  %12 = zext i1 %11 to i32
  %13 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i32 %12)
  ret i32 0
}

declare void @_ZN6TripleIPcibE4ctorERPcRiRb(ptr, ptr, ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_ZN6TripleIPcibE8getFirstEv(ptr) local_unnamed_addr

declare ptr @_ZN6TripleIPcibE9getSecondEv(ptr) local_unnamed_addr

declare ptr @_ZN6TripleIPcibE8getThirdEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
