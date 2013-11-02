#include "monster.h"
#include "math.h"
#include "player.h"
#include "level.h"
#include "time.h"

Monster::Monster( Player* p, Level* l, unsigned char monster_id, const float* pos )
{
    player= p;
    level= l;

    this->monster_id= monster_id;

    this->pos[0]= pos[0];
    this->pos[1]= pos[1];
    this->pos[2]= pos[2];

    health= monster_descriptor_table[ monster_id ].max_health;
    last_attack_time= 0;
    last_target_select_time= 0;

    for( int i= 0; i< ANIMATION_KIND; i++ )
    {
        playable_animations[i]= false;
        playable_animations_time[i]= 0.0f;
    }

    in_melee_attack= false;
    alrady_caused_damage= false;
}

void Monster::Attack()
{
    unsigned int current_time= clock();
    float dt= float( current_time - last_attack_time ) / float( CLOCKS_PER_SEC );

    MonsterDescriptor* md= &monster_descriptor_table[monster_id];
    if( dt > md->melee_attack_delay )
    {
        last_attack_time= current_time;
        alrady_caused_damage= false;
        in_melee_attack= true;
        playable_animations[ANIMATION_ATTACK0]= true;
        playable_animations_time[ANIMATION_ATTACK0] = 0.0f;
    }
}


void Monster::AttackTick()
{
    unsigned int current_time= clock();
    MonsterDescriptor* md= &monster_descriptor_table[monster_id];
    float a_dt= float( current_time - last_attack_time ) / float( CLOCKS_PER_SEC );
    if( in_melee_attack )
    {
        if ( a_dt > md->delay_before_melee_attack && a_dt <=  md->delay_before_melee_attack + md->delay_after_melee_attack )
        {
            if( ! alrady_caused_damage )
            {
                alrady_caused_damage= true;
                if(  Distance( pos, player->Position() ) < 2.0f )
                    player->Damage( md->melee_damage );
            }
        }
        else if( a_dt > md->delay_before_melee_attack + md->delay_after_melee_attack )
        {
           in_melee_attack= false;
           playable_animations[ANIMATION_ATTACK0]= false;
        }
    }
}

void Monster::SelectTarget()
{
    unsigned int current_time= clock();
    if( current_time - last_target_select_time > CLOCKS_PER_SEC )
    {
        last_target_select_time= current_time;
        target_pos[0]= player->Position()[0];
        target_pos[1]= player->Position()[1];
        target_pos[2]= player->Position()[2] + PLAYER_HEIGHT; 
    }
}

void Monster::Tick( float dt )
{
    MonsterDescriptor* md= &monster_descriptor_table[monster_id];
    AttackTick();
    SelectTarget();
    float vec_to_target[]= { target_pos[0], target_pos[1], target_pos[2] };
    Vec3Sub( vec_to_target, pos );
        
    if( Vec3Len( vec_to_target) < 2.0f  )
    {
        Attack();
        playable_animations_time[ANIMATION_WALK0]= 0.0f;
        playable_animations[ ANIMATION_WALK0 ]= false;
    }
    else
    {
        Vec3Normalize( vec_to_target );

        Vec3Mul( vec_to_target, dt * md->speed );
        Vec3Add( pos, vec_to_target );

        angle_xy= Vec3XYAngle( vec_to_target ) - MW_PI2;
        playable_animations[ ANIMATION_WALK0 ]= true;
    }

    for( int i= 0; i< MAX_MODEL_ANIMATIONS; i++ )
         playable_animations_time[i]+= dt;

    if( ! MONSTER_CAN_FLY( md->flags ) )
        pos[2]-= dt * 2.0f;
    
    float h= level->GetLandscape()->Height( pos[0], pos[1] );
    float dh= 0.0f;
    if( MONSTER_IS_SPHERICAL(md->flags) )
        dh= md->radius;
    if( pos[2] - dh < h )
        pos[2]= h + dh;


}

bool Monster::IsCollision( const float* pos, const float* vec, float* collision_pos )
{
    float vec2_monster[3];
    float vec2_near_point[3];
    float dot, l1, l2;

  //  bool is_cylindrical= MONSTER_IS_CYLINDRICAL(monster_descriptor_table[monster_id].flags);

    vec2_monster[0]= this->pos[0];
    vec2_monster[1]= this->pos[1];
    vec2_monster[2]= this->pos[2];
    Vec3Sub( vec2_monster, pos );
    
    dot= Vec3Dot( vec2_monster, vec );

    vec2_near_point[0]= dot * vec[0];
    vec2_near_point[1]= dot * vec[1];
    vec2_near_point[2]= dot * vec[2];

    l1= Vec3Len( vec2_monster );
    l2= Vec3Len( vec2_near_point );

    if( monster_descriptor_table[monster_id].radius > sqrt( l1*l1 - l2*l2 ) )
    {
        collision_pos[0]= pos[0] + vec2_near_point[0];
        collision_pos[1]= pos[1] + vec2_near_point[1];
        collision_pos[2]= pos[2] + vec2_near_point[2];
       // if( ! is_cylindrical )
            return true;
       // else 
      //  {
      //     return true;
      //  }
    }

    return false;
    
}