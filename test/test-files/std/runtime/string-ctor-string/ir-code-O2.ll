; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [592 x i8] c"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Capacity: %d\00", align 1

define i32 @main() local_unnamed_addr {
entry.l1:
  %testString = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor(ptr nonnull %testString)
  call void @_mp__String__void__dtor(ptr nonnull %testString)
  call void @_mp__String__void__ctor__string(ptr nonnull %testString, ptr nonnull @0)
  %0 = call i64 @_mf__String__long__getLength(ptr nonnull %testString)
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @1, i64 %0)
  %2 = call i64 @_mf__String__long__getCapacity(ptr nonnull %testString)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @2, i64 %2)
  call void @_mp__String__void__dtor(ptr nonnull %testString)
  ret i32 0
}

declare void @_mp__String__void__ctor(ptr) local_unnamed_addr

declare void @_mp__String__void__dtor(ptr) local_unnamed_addr

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare i64 @_mf__String__long__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getCapacity(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
